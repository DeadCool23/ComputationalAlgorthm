import math
import numpy as np

EPS = 1e-8

def f(x):
    return (1 / math.sqrt(2 * math.pi)) * math.exp(-(x ** 2) / 2)


def simpson(xCur):
    steps = 1000
    step = xCur / (steps * 2)

    xValues = np.array([i * step for i in range(steps * 2 + 1)])
    funcValues = np.array([f(x_i) for x_i in xValues])

    sumX1 = 4 * np.sum([funcValues[2 * i - 1] for i in range(1, steps)])
    sumX2 = 2 * np.sum([funcValues[2 * i] for i in range(1, steps - 1)])

    return (step / 3) * (funcValues[0] + funcValues[-1] + sumX1 + sumX2)

def bin_search(value):
    left = 0
    right = 1

    while value > simpson(right):
        left = right
        right *= 2

    while right - left > EPS:
        midX = (right + left) / 2

        if value < simpson(midX):
            right = midX
        else:
            left = midX

    return left
    

def golden_section_search(value):
    left = 0
    right = 1
    
    while value > simpson(right):
        left = right
        right *= 2

    phi = (1. + math.sqrt(5.)) / 2.
    resphi = 2. - phi

    x1 = left + resphi * (right - left)
    x2 = right - resphi * (right - left)

    f1, f2 = simpson(x1), simpson(x2)

    while abs(right - left) > EPS:
        if abs(f1 - value) < abs(f2 - value):
            right = x2
            x2, f2 = x1, f1
            x1 = left + resphi * (right - left)
            f1 = simpson(x1)
        else:
            left = x1
            x1, f1 = x2, f2
            x2 = right - resphi * (right - left)
            f2 = simpson(x2)

    return (x1 + x2) / 2.