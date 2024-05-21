def print_coeffs(coeffs_values):
    print("Результаты подсчетов коэффициент:")
    for i in range(len(coeffs_values)):
        print("a" + str(i) + " = {:10.6g}".format(coeffs_values[i]))


def print_matrix(matrix):
    for row in matrix:
        for el in row:
            if abs(el) < 1e-6:
                el = 0.0
            print("{:15.6g}".format(el), end=" ")
        print()
