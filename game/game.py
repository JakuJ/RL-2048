import random
import numpy as np

def swipe(direction, matrix, score):   
    size = matrix.shape[0]
    dx, dy, enum= {
        'up': (0, 1, [(y, x) for x in range(size) for y in range (size - 1)]),
        'down': (0, -1, [(y, x) for x in range(size) for y in range (size - 1, 0, -1)]),
        'left': (1, 0, [(y, x) for y in range(size) for x in range (size - 1)]),
        'right': (-1, 0, [(y, x) for y in range(size) for x in range (size - 1, 0, -1)])
    }[direction]   
    move_done = slide(direction, matrix)
    for y, x in enum:
        if not 0 <= y + dy < size:
            break
        if not 0 <= x + dx < size:
            break
        if matrix[y, x] == 0:
            continue
        if matrix[y, x] == matrix[y + dy, x + dx]:
            matrix[y, x] *= 2
            matrix[y + dy, x + dx] = 0
            score += matrix[y, x]
            x += dx
            y += dy
            move_done = True
    if not move_done:
        return True
    slide(direction, matrix)
    generate_new_tile(matrix)
    return score

def get_highest_tile(matrix):
    return max([y for x in matrix for y in x])

def slide(direction, matrix):
    size = matrix.shape[0]
    dx, dy, enum= {
        'up': (0, 1, [(y, x) for x in range(size) for y in range (size - 1)]),
        'down': (0, -1, [(y, x) for x in range(size) for y in range (size - 1, 0, -1)]),
        'left': (1, 0, [(y, x) for y in range(size) for x in range (size - 1)]),
        'right': (-1, 0, [(y, x) for y in range(size) for x in range (size - 1, 0, -1)])
    }[direction]   
    move_done = False
    for y, x in enum:
        j = y
        i = x
        while 0 <= j < size and 0 <= i <size and matrix[j, i] == 0 and matrix[j + dy, i + dx] != 0:
            matrix[j, i] = matrix[j + dy, i + dx]
            matrix[j + dy, i + dx] = 0
            j -= dy
            i -= dx
            move_done = True
    return move_done

def generate_new_tile(matrix):
    size = matrix.shape[0]
    x = random.randrange(size)
    y = random.randrange(size)
    while matrix[y, x]:
        x = random.randrange(size)
        y = random.randrange(size)
    matrix[y, x] = 2 if random.random() < 0.9 else 4
    check_if_gameover(matrix)

def check_if_gameover(matrix):
    if any(0 in row for row in matrix):
        return False
    size = matrix.shape[0]
    for x in range(size):
        for y in range(size):
            if y < size - 1 and matrix[y, x] == matrix[y+1, x]:
                return False
            if x < size - 1 and matrix[y, x] == matrix[y, x+1]:
                return False
    return True   

def predict_moves(matrix, score=0):
    dictionary = {}
    for move in possible_moves(matrix):
        arr = np.copy(matrix)
        scr = swipe(move, arr, score)
        dictionary[move] = scr
    return dictionary

def new_board(size=4):
    matrix = np.zeros((size, size))
    generate_new_tile(matrix)
    return matrix

def heuristic(matrix, heuristic_matrix):
    # maybe function body and arguents should be changed
    # sample heuristic_matrix for 2048
    # size = matrix.shape[0]
    # heuristic_matrix = np.zeros((size, size))
    # a = 1.5
    # for i in range(size):
    #     for j in range(size):
    #         if i % 2:
    #             heuristic_matrix[i, j] = a ** ((i + 1) * size - j)
    #         else:
    #             heuristic_matrix[i, j] = a ** (i * size + j + 1)             

    return np.sum(np.multiply(matrix, heuristic_matrix))


def possible_moves(matrix):
    possible = set()
    size = matrix.shape[0]
    zeros = np.zeros((size,))
    for row in matrix:
        if all(row == zeros):
            continue
        if any(x[0]==x[1] and x[0] != 0 and x[1] !=0 for x in zip(row,row[1:])):
            possible.update(['left','right'])
            continue
        isZeroInTheMiddle = any(row[1:-1]==zeros[1:-1])
        if row[0] == 0 or (row[0]!=0 and any(row[1:]!=zeros[1:]) and isZeroInTheMiddle):
            possible.add('left')
        if row[-1] == 0 or (row[-1]!=0 and any(row[:-1]!=zeros[1:]) and isZeroInTheMiddle):
            possible.add('right')

    for i in range(size):
        col = matrix[:,i]
        if all(col == zeros):
            continue
        if any(x[0]==x[1] and x[0] != 0 and x[1] !=0 for x in zip(col,col[1:])):
            possible.update(['up','down'])
            continue
        isZeroInTheMiddle = any(col[1:-1]==zeros[1:-1])
        if col[0] == 0 or (col[0]!=0 and any(col[1:]!=zeros[1:]) and isZeroInTheMiddle):
            possible.add('up')
        if col[-1] == 0 or (col[-1]!=0 and any(col[:-1]!=zeros[1:]) and isZeroInTheMiddle):
            possible.add('down')
        
    return possible