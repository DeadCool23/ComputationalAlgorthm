import copy
import numpy as np
from src.Point import *
from src.printers import *
from src.gauss import gauss_solution
from matplotlib import pyplot as plt

def least_squares_coeffs_3D(data: list[Point], n):
    dataLen = len(data)

    def aSum(xdeg, ydeg):
        return sum([point.x ** xdeg * point.y ** ydeg * point.weight for point in data])
    
    def zSum(xdeg, ydeg):
        return sum([point.x ** xdeg * point.y ** ydeg * point.z * point.weight for point in data])
    
    aCoeffs = [[aSum(i + k, j + t) for k in range(n + 1) for t in range(n + 1 - k) ] for i in range(n + 1) for j in range(n + 1 - i) ]
    zCoeffs = [zSum(i, j) for i in range(n + 1) for j in range(n + 1 - i) ]

    Matrix = [copy.deepcopy(aCoeffs[i]) + [copy.deepcopy(zCoeffs[i])] for i in range(len(aCoeffs))]
    return gauss_solution(Matrix)

def least_square_3D(data, n, print_err=False, ret_coefs=False):
    coefs = least_squares_coeffs_3D(data, n)
    def func(xp, yp):
        res = 0
        ind = 0
        for i in range(n + 1):
            for j in range(n + 1 - i):
                res += coefs[ind] * xp ** i * yp ** j
                ind += 1
        return res
    
    sum_err = 0
    if print_err:
        for point in data:
            R = point.weight * (func(point.x, point.y) - point.z) ** 2
            sum_err += R
        print(f"I: {sum_err}")
    
    return [func, coefs] if ret_coefs else func

def parseTableToCoordinates3D(pointTable):
    xs = list()
    ys = list()
    zs = list()
    for p in pointTable:
        xs.append(p.y)
        ys.append(p.y)
        zs.append(p.z)
    return np.array(xs), np.array(ys), np.array(zs)


def fgraph_3D(func, data, color='blue', axes=None):
    x_min, x_max = get_interval_x(data)
    y_min, y_max = get_interval_y(data)

    dots_count_x = 40
    x_values = np.linspace(x_min, x_max, dots_count_x)
    
    dots_count_y = 40
    y_values = np.linspace(y_min, y_max, dots_count_y)
    
    z_values = [func(x_values[i], y_values[i])
               for i in range(len(x_values))]

    def make_3D_matrix():
        x_grid, y_grid = np.meshgrid(x_values, y_values)
        z_grid = np.array([
            [
                func(
                    x_grid[i][j],
                    y_grid[i][j],
                ) for j in range(len(x_values))
            ] for i in range(len(y_values))
        ])
        return x_grid, y_grid, z_grid

    if axes is None:
        fig = plt.figure(
            "График функции, полученный аппроксимации наименьших квадратов")
        xpoints, ypoints, zpoints = parseTableToCoordinates3D(data)
        
        axes = fig.add_subplot(projection='3d')
        axes.scatter(xpoints, ypoints, zpoints, c='red')
        axes.set_xlabel('OX')
        axes.set_ylabel('OY')
        axes.set_zlabel('OZ')
    
    x_values, y_values, z_values = make_3D_matrix()
    axes.plot_surface(x_values, y_values, z_values, color=color)
    return axes
