import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.optimize import curve_fit
from typing import Callable
from itertools import product, permutations, combinations
from matplotlib.colors import ListedColormap

def plot_matrix(matrix: np.ndarray):

    #cmap = ListedColormap(['white', 'black'])
    fig, ax = plt.subplots(figsize = (4,4))
    ax.imshow(matrix) #, cmap = cmap)
    ax.set_xticks([]); ax.set_yticks([])
    plt.show()

T = np.array([[1, 1, 1, 1, 1],
            [-1, -1, 1, -1, -1],
            [-1, -1, 1, -1, -1],
            [-1, -1, 1, -1, -1],
            [-1, -1, 1, -1, -1]])

H = np.array([[1, -1, -1, -1, 1],
            [1, -1, -1, -1, 1],
            [1, 1, 1, 1, 1],
            [1, -1, -1, -1, 1],
            [1, -1, -1, -1, 1]])

A = np.array([[-1, -1, 1, -1, -1],
            [-1, 1, -1, 1, -1],
            [-1, 1, -1, 1, -1],
            [1, 1, 1, 1, 1],
            [1, -1, -1, -1, 1]])

E = np.array([[1, 1, 1, 1, 1],
            [1, -1, -1, -1, -1],
            [1, 1, 1, 1, 1],
            [1, -1, -1, -1, -1],
            [1, 1, 1, 1, 1]])

Tb = np.array([[1, 1, 1, -1, 1],
            [-1, -1, 1, -1, -1],
            [-1, -1, -1, -1, -1],
            [1, -1, -1, -1, -1],
            [-1, -1, 1, -1, -1]])

Hb = np.array([[1, -1, -1, -1, 1],
            [1, -1, -1, -1, 1],
            [1, -1, 1, 1, -1],
            [1, -1, -1, -1, -1],
            [1, -1, -1, -1, 1]])

Ab = np.array([[-1, -1, -1, -1, -1],
            [-1, 1, -1, 1, -1],
            [-1, 1, -1, 1, -1],
            [-1, -1, -1, -1, -1],
            [1, -1, -1, -1, 1]])

def make_w(matrices: list) -> np.ndarray:
    
    matrices_1D = [m.flatten() for m in matrices]
    data_size = len(matrices_1D[0])
    w = np.zeros((data_size, data_size))

    for matrix_1D in matrices_1D:
        for i in range(data_size):
            for j in range(data_size):
                w[i,j] += matrix_1D[i]*matrix_1D[j]
                if i == j:
                    w[i,j] = 0

    return w

def w_matrix(w: np.ndarray, matrix: np.ndarray) -> np.ndarray:
    shape = matrix.shape
    matrix1D = matrix.flatten()

    wmatrix_new = matrix1D.copy()
    wmatrix = np.zeros_like(matrix1D)

    while not np.array_equal(wmatrix_new, wmatrix):
        wmatrix = wmatrix_new.copy()
        wmatrix_new = np.sign(np.dot(w, wmatrix))
        wmatrix_new[wmatrix_new == 0] = 1

    wmatrix = np.reshape(wmatrix, shape)
    return wmatrix

plot_matrix(Ab)
w = make_w([H, A, E])
print(w)

#w = make_w([T, H, A, E])
#plot_matrix(w_matrix(w, Ab))