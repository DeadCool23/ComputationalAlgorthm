def to_triangle(matrix):
    n = len(matrix)
    for k in range(n):
        for i in range(k + 1, n):
            coeff = -(matrix[i][k] / matrix[k][k])
            for j in range(k, n + 1):
                matrix[i][j] += coeff * matrix[k][j]

    return matrix


def gauss_solution(matrix):
    n = len(matrix)

    matrix = to_triangle(matrix)

    result = [0.0 for _ in range(n)]
    for i in range(n - 1, -1, -1):
        for j in range(n - 1, i, -1):
            matrix[i][n] -= result[j] * matrix[i][j]

        result[i] = matrix[i][n] / matrix[i][i]
    return result