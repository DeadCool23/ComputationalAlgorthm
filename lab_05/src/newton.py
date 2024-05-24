import numpy as np

def Newton(jacobian, sys_func, start_approx, eps=1e-5):
    n_vars = len(start_approx)
    coefs = jacobian(*start_approx)
    fn_values = -sys_func(*start_approx)

    deltas = np.linalg.solve(coefs, fn_values).reshape((n_vars,))

    approx = start_approx + deltas

    while any(map(lambda x: abs(x) >= eps, deltas / approx)):
        coefs = jacobian(*approx)
        fn_values = -sys_func(*approx)

        deltas = np.linalg.solve(coefs, fn_values).reshape((n_vars,))

        approx += deltas

    return approx
