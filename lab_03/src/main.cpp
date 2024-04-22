#include <iostream>

#include "graph.hpp"
#include "funcs_gen.hpp"
#include "points_4d.hpp"
#include "mult_interpolation.hpp"

#define GRAPH_RANGES {10, 10, 10}

#define X_PARAMS std::pair<Interval, double>({0, 4}, 5)
#define Y_PARAMS std::pair<Interval, double>({0, 4}, 5)
#define Z_PARAMS std::pair<Interval, double>({0, 4}, 5)

#define INTERVALS std::vector<std::pair<Interval, double>>{ X_PARAMS, Y_PARAMS, Z_PARAMS }

#define GEN_FUNC test_func2

int main(void) {
    auto table4d = Points4D(INTERVALS, GEN_FUNC);

    std::vector<size_t> ndegs;
    ndegs.resize(3);
    std::vector<char> Ograph{'x', 'y', 'z'};
     for (size_t i = 0; i < 3; ++i) {
        std::cout << "Введите степень апроксимации n" << Ograph[i] << ": ";
        try {
            std::cin >> ndegs[i];
        } catch (...) {
            std::cout << "Ошибка ввода" << std::endl;
            return -127;
        };
    }
    std::cout << std::endl;

    Point3D point;
    std::cout << "Введите точку для апроксимации {x y z}: ";
    try {
        std::cin >> point.x >> point.y >> point.z;
    } catch (...) {
        std::cout << "Ошибка ввода" << std::endl;
        return -127;
    }

    std::cout << std::endl;

    table4d.print_table();

    std::cout << std::endl;
    std::string func = "f(x, y, z)";
    std::cout << "Newton result: " << std::endl;
    std::cout << func << " = " << mult_interpolation(table4d, point, ndegs, NEWTON) << std::endl;

    std::cout << "Splines result: " << std::endl;
    std::cout << func << " = " << mult_interpolation(table4d, point, ndegs, SPLINE) << std::endl;

    std::cout << "Mixed result: " << std::endl;
    std::cout << func << " = " << mult_interpolation(table4d, point, ndegs, MIXED) << std::endl;

    char is_drawing = 'y';
    std::cout << std::endl << "Draw graphics? [Y/N]: ";
    if (std::cin >> is_drawing) {
        is_drawing = std::tolower(is_drawing);
        if (is_drawing == 'n') is_drawing = 'n';
        else is_drawing = 'y';
    } else {
        is_drawing = 'y';
    }

    if (is_drawing == 'y') graph_draw(table4d, GRAPH_RANGES, "Func4D", false);

    return ~0xFFFFFFFF;
}
