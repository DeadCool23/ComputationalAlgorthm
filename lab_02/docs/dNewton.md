# Исследование разных степеней полинома Ньютона

#### \(P_n(x)\):

\[
P_n(x) = c_1 + c_2(x - x_1) + c_3(x - x_1)(x - x_2) + \ldots + c_n(x - x_1)(x - x_2) \ldots (x - x_{n - 1})
\]

---

#### \(P_n'(x)\):

<!-- (x - x1)' = 1
((x - x1))(x - x2)' = (x - x1) + (x - x2)
((x - x1)(x - x2))(x - x3)' = ((x - x1) + (x - x2))(x - x3) + (x - x1)(x - x2)
(((x - x1)(x - x2))(x - x3))(x - x4)' = (((x - x1) + (x - x2))(x - x3) + (x - x1)(x - x2))(x - x4) + (x - x1)(x - x2)(x - x3)
((((x - x1)(x - x2))(x - x3))(x - x4))(x - x5)' = ((((x - x1) + (x - x2))(x - x3) + (x - x1)(x - x2))(x - x4) + (x - x1)(x - x2)(x - x3))(x - x5) + (x - x1)(x - x2)(x - x3)(x - x4) -->

\(
Пусть  (x - x_1) \ldots (x - x_n) = f_n(x)
\)

\(
Тогда: f_n'(x) = \sum_{i=1}^{n} \prod_{j=1,(j \ne i)}^{n}(x - x_j)
\)

\[
Следовательно: \\ P_n'(x) = c_2 + c_3 f_2'(x) + \ldots + c_n f_{n-1}'(x)
\]

---

#### \(P_n^{(m)}(x)\):

\(
Пусть \  (x - x_1) \ldots (x - x_n) = f_n(x) = \prod_{i = 1}^n (x - x_i)
\)

\(
Тогда:\\
f_n'(x) = \sum_{i=1}^{n} \prod_{j=1,(j \ne i)}^{n}(x - x_j) \\
f_n''(x) = 2! \cdot \sum_{i_1 = 1}^{n} \sum_{i_2 = i_1 + 1}^{n} \prod_{j = 1, j \notin \{i_1, i_2\}}^n (x - x_k) \\
f_n'''(x) = 3! \cdot \sum_{i_1 = 1}^{n} \sum_{i_2 = i_1 + 1}^{n} \sum_{i_3 = i_2 + 1}^{n}\prod_{j = 1, j \notin \{i_1, i_2, i_3\}}^n (x - x_k) \\ \\
\ldots \\
f_n^{(m)}(x) = m! \cdot \sum_{i_1 = 1}^{n} (\sum_{i_2 = i_1 + 1}^{n} \ldots ( \sum_{i_m =  i_{m-1}+1\}}^{n} \prod_{j = 1, j \notin \{i_1, i_2, \ldots,i_m\}}^n (x - x_j)))
\)

<br>

\[Следовательно: \\
P_n^{(m)}(x) = m!  \cdot c_{m + 1} + c_{m + 2} f_{m + 1}^{(m)}(x) + \ldots + c_n f_{n-1}^{(m)}(x)
\]
