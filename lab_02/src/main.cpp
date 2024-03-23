#include <cmath>
#include <limits>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "errs.h"
#include "graph.hpp"
#include "newton.hpp"
#include "splines.hpp"

extern "C" {
    #include "ctype.h"
    #include "string.h"
}

PointsTable build_table(std::function<double(double)> func, std::pair<double, double> bords, double step);

int main(int argc, char **argv) {
    errs_t err = OK;
    std::ifstream file(argc < 2 ? "data/std_data.txt" : argv[1]);
    if (!file.is_open()) {
        print_errs(err = ERR_FILE);
        return err;
    }
    
    auto table = PointsTable(file);
    if ((err = table.is_incorrect())) {
        print_errs(err);
        return err;
    }
    file.close();

    double bords[] = {
        [0] = 0,
        [1] = (NewtonPolinom(table.point_range(table[0].x, 3)).diff().diff())(table[0].x),
        [2] = (NewtonPolinom(table.point_range(table[table.size() - 1].x, 3)).diff().diff())(table[table.size() - 1].x)
    };

    auto zzSpline = Splines(table);
    auto nzSpline = Splines(table, std::pair(bords[1], 0));
    auto nnSpline = Splines(table, std::pair(bords[1], bords[2]));

    double x = 0;
    std::cout << "Input x: ";
    if (!(std::cin >> x)) {
        print_errs(err = ERR_IO);
        return err;
    };

    std::cout << "P_3(" << x << ") = " << std::setprecision(10) << NewtonPolinom(table.point_range(x, 3))(x) << std::endl;
    std::cout << "Sp(" << x << ") = " << std::setprecision(10) << zzSpline(x) << " \\\\ [Phi''(x_0) == 0 & Phi''(x_N) == 0]" << std::endl;
    std::cout << "Sp(" << x << ") = " << std::setprecision(10) << nzSpline(x) << " \\\\ [Phi''(x_0) == P_3''(x_0) & Phi''(x_N) == 0]" << std::endl;
    std::cout << "Sp(" << x << ") = " << std::setprecision(10) << nnSpline(x) << " \\\\ [Phi''(x_0) == P_3''(x_0) & Phi''(x_N) == P_3''(x_N)]" << std::endl;

    char is_drawing = 'y';
    std::cout << std::endl << "Draw graphics? [Y/N]: ";
    if (std::cin >> is_drawing) {
        is_drawing = std::tolower(is_drawing);
        if (is_drawing == 'n') is_drawing = 'n';
        else is_drawing = 'y';
    } else {
        is_drawing = 'y';
    }

    double step = 1e-3;
    if (step && is_drawing == 'y') {
        auto bords = std::pair(table[0].x, table[table.size() - 1].x);
        std::vector<PointsTable> tables = {
            table,
            build_table([table](double x) -> double {
                return NewtonPolinom(table.point_range(x, 3))(x);
            }, bords, step),
            build_table([zzSpline](double x) -> double {
                return zzSpline(x);
            }, bords, step),
            build_table([nzSpline](double x) -> double {
                return nzSpline(x);
            }, bords, step),
            build_table([nnSpline](double x) -> double {
                return nnSpline(x);
            }, bords, step),
        };
        std::vector<std::string> lbls = {
            "GivenTable",
            "ThirdDegreeNewton",
            "StdSpline",
            "StartBoadSpline",
            "AllBoardSpline",
        };
        graphs_draw(tables, lbls);
    } else if (!step) {
        print_errs(err = table.is_incorrect());
    }

    return err;
}

PointsTable build_table(std::function<double(double)> func, std::pair<double, double> bords, double step) {
    PointsTable new_table;
    for (double x = bords.first; x <= bords.second; x += step)
        new_table.push_back(Point({
            .x = x,
            .y = func(x)
        }));
    return new_table;
}