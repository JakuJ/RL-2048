from game import *


def unit():
	board = new_board()
	score = 0
	while not check_if_gameover(board):
		move = possible_moves(board).pop()
		board, plus = swipe(board, move)
		score += plus
	print(board)


def unit_greedy():
	board = new_board()
	score = 0
	while not check_if_gameover(board):
		moves = predict_moves(board)
		plus = 0
		for b, p in moves.values():
			if p >= plus:
				board = b
				plus = p

		score += plus
	print(board)


def snake_heuristic(size):
	heuristic_matrix = np.zeros((size, size))
	a = 1.4141
	for i in range(size):
		for j in range(size):
			if i % 2:
				heuristic_matrix[i, j] = a ** ((i + 1) * size - j)
			else:
				heuristic_matrix[i, j] = a ** (i * size + j + 1)

	return heuristic_matrix


def heuristic(matrix, heuristic_matrix):
	return np.sum(np.multiply(matrix, heuristic_matrix))


def unit_heuristic():
	board = new_board()
	hm = snake_heuristic(board.shape[0])

	while not check_if_gameover(board):
		moves = predict_moves(board)
		state = 0
		for b, _ in moves.values():
			s = heuristic(b, hm)
			if s >= state:
				board = b
				state = s

	print(board)
