import copy
import numpy as np
from src.Point import *
from src.printers import *
from src.gauss import gauss_solution
from matplotlib import pyplot as plt

def lst_squares_coefs_2D(data: list[Point], n):
    dataLen = len(data)

    aCoeffsx = [sum([data[i].weight * data[i].x ** j for i in range(dataLen)]) for j in range(2 * n + 1)]
    aCoeffsy = [sum([data[i].weight * data[i].y * data[i].x ** j for i in range(dataLen)]) for j in range(n + 1)]
    CoeffMat = [[aCoeffsx[i + j] for i in range(n + 1)] for j in range(n + 1)]

    Matrix = [copy.deepcopy(CoeffMat[i]) + [copy.deepcopy(aCoeffsy[i])] for i in range(len(CoeffMat))]
    return gauss_solution(Matrix)

def least_square_2D(data, n, print_err=False, ret_coefs=False):
    coefs = lst_squares_coefs_2D(data, n)
    def func(xp):
        return sum([coefs[i] * xp ** i for i in range(n + 1)])
    
    sum_err = 0
    if print_err:
        for point in data:
            R = point.weight * (func(point.x) - point.y) ** 2
            sum_err += R
        print(f"I: {sum_err}")
    
    return [func, coefs] if ret_coefs else func


def fgraph_2D(func, data, color='blue', label="y = f(x)"):
    x_min, x_max = get_interval_x(data)
    dots_count = int(x_max - x_min + 1) * 100
    x_values = np.linspace(x_min, x_max, dots_count)

    for p in data:
        plt.plot(p.x, p.y, 'r.')

    y_values = np.array([func(xi) for xi in x_values])
    plt.plot(x_values, y_values, color=color, label=label)
    plt.legend()
    