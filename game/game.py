import numpy as np


def swipe(board, direction):
	rots = {
		'up'   : 0,
		'right': 1,
		'down' : 2,
		'left' : 3,
	}[direction]

	rotated = np.rot90(board, rots)
	score = swipe_up(rotated)
	board = np.rot90(rotated, 4 - rots)
	return board, score


def swipe_up(board):
	move_done = slide_up(board)
	size = board.shape[0]
	score = 0

	for x in range(size):
		for y in range(size - 1):
			if y >= size - 1:
				break

			if board[y][x] == 0:
				continue

			if board[y][x] == board[y + 1][x]:
				board[y][x] *= 2
				board[y + 1][x] = 0
				score += board[y][x]
				y += 1
				move_done = True

	if not move_done:
		return score

	slide_up(board)
	generate_new_tile(board)

	return score


def slide_up(board):
	move_done = False
	size = board.shape[0]

	for x in range(size):
		for y in range(size - 1):
			i = y
			while i >= 0 and board[i][x] == 0 and board[i + 1][x] != 0:
				board[i][x] = board[i + 1][x]
				board[i + 1][x] = 0
				i -= 1
				move_done = True
	return move_done


def get_highest_tile(matrix):
	return np.max(matrix)


def generate_new_tile(matrix):
	size = matrix.shape[0]
	x = np.random.randint(0, size)
	y = np.random.randint(0, size)

	while matrix[y, x]:
		x = np.random.randint(0, size)
		y = np.random.randint(0, size)

	matrix[y, x] = 2 if np.random.uniform() < 0.9 else 4


def check_if_gameover(matrix):
	if np.min(matrix) == 0:
		return False

	size = matrix.shape[0]
	for x in range(size):
		for y in range(size):
			if y < size - 1 and matrix[y, x] == matrix[y + 1, x]:
				return False
			if x < size - 1 and matrix[y, x] == matrix[y, x + 1]:
				return False
	return True


def predict_moves(matrix):
	return {move: swipe(np.copy(matrix), move) for move in possible_moves(matrix)}


def new_board(size=4):
	matrix = np.zeros((size, size))
	generate_new_tile(matrix)
	return matrix


def possible_moves(matrix):
	possible = set()

	def check_lr(mat, left, right):
		can_left = False
		can_right = False

		for row in mat:
			for x, y in zip(row[:-1], row[1:]):
				can_left |= (x == 0 and y != 0) or (x != 0 and x == y)
				can_right |= (x != 0 and y == 0) or (x != 0 and x == y)

		if can_left:
			possible.add(left)

		if can_right:
			possible.add(right)

	check_lr(matrix, 'left', 'right')
	check_lr(np.rot90(matrix), 'up', 'down')

	return possible
