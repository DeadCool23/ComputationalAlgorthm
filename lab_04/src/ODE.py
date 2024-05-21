import copy
import numpy as np
from scipy.special import erfi
from src.gauss import gauss_solution

funcs = [lambda x, i=i: x ** i * (1 - x) for i in range(4)]
cfuncs = [
    lambda x: 1 - 4 * x,
    lambda x: -2 + 2 * x - 3 * x**2,
    lambda x: 2 - 6 * x + 3 * x**2 - 4 * x**3,
    lambda x: 6 * x - 12 * x**2 + 4 * x**3 - 5 * x**4
]

def ODESolution(x):
    return ((-(np.exp(0.5) + 1) / erfi(1 / np.sqrt(2))) * erfi(x / np.sqrt(2)) + 1) / np.exp(x ** 2 / 2) + x

def ApproximateODE(x, n, funcs, coeffFuncs):
    if (len(funcs) != n + 1 or len(coeffFuncs) != n + 1):
        raise ValueError("Number of functions must equal n + 1")
    
    f0 = coeffFuncs[0]
    cCoeffs = [[sum([f1(xp) * f2(xp)  for xp in x]) for f2 in coeffFuncs[1:]] for f1 in coeffFuncs[1:]]
    fCoeffs = [-sum([f0(xp) * f(xp) for xp in x])  for f in coeffFuncs[1:]]

    Matrix = [copy.deepcopy(cCoeffs[i]) + [copy.deepcopy(fCoeffs[i])] for i in range(len(cCoeffs))]
    poliCoeffs = gauss_solution(Matrix)
    
    def func(_x):
        return funcs[0](_x) + sum([poliCoeffs[i] * funcs[i + 1](_x) for i in range(n)])

    return func, poliCoeffs