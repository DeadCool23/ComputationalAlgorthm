#include <limits>
#include <iostream>
#include <functional>

#include "errs.h"
#include "graph.hpp"
#include "interpol.hpp"
#include "point_table.hpp"

#define STD_STEP 0.1

template <typename T>
std::vector<T> operator+(const std::vector<T>&, const std::vector<T>&);

int main(void) {
    errs_t err = OK;
    std::ifstream fst_func("./data/functions/fst.txt");
    std::ifstream scd_func("./data/functions/scd.txt");
    if (!(fst_func.is_open() || scd_func.is_open())) {
        print_errs(err = ERR_FILE);
        return err;
    }

    auto fst_table = PointsTable(fst_func).reverse_func();
    auto scd_table = PointsTable(scd_func);

    fst_func.close();
    scd_func.close();

    if (fst_table.is_incorrect() || scd_table.is_incorrect()) {
        print_errs(err = ERR_IO);
        return err;
    }

    auto f = newton_polinom(fst_table);
    auto g = newton_polinom(scd_table);

    if (!(f && g)) {
        print_errs(err = ERR_POLINOM);
        return err;
    }

    auto dx = fst_table.get_xs() + scd_table.get_xs();
    std::sort(dx.begin(), dx.end());

    auto mm_dx = std::pair(dx[0], dx[dx.size() - 1]);

    PointsTable fg_diff;
    auto f_x = fst_table.get_xs();
    auto g_x = scd_table.get_xs();
    for (auto x : dx) {
        auto finder_f_x = std::find(f_x.begin(), f_x.end(), x);
        auto finder_g_x = std::find(g_x.begin(), g_x.end(), x);
        fg_diff.push_back(Point {
            .x = x, 
            .y = (finder_f_x == f_x.end() ? (*f)(x) : fst_table[std::distance(f_x.begin(), finder_f_x)].y) -
                 (finder_g_x == g_x.end() ? (*g)(x) : scd_table[std::distance(g_x.begin(), finder_g_x)].y),
            .dy = std::vector<double>(0)
        });
    }


    auto fg_diff_fn = newton_polinom(fg_diff);
    if (!fg_diff_fn) {
        print_errs(err = ERR_CALC);
        return err;
    }

    err = graphs_draw(std::pair(*f, *g), mm_dx, STD_STEP, false);
    err = !err ? graph_draw(*fg_diff_fn, mm_dx, STD_STEP, false, "Funcions difference") : err;
    if (err) {
        print_errs(err);
        return err;
    }

    auto inv_fg_diff = fg_diff.reverse_func();
    inv_fg_diff = inv_fg_diff.point_range(0, 5);

    auto inv_fg_diff_fn = newton_polinom(inv_fg_diff);
    if (!inv_fg_diff_fn) {
        print_errs(err = ERR_CALC);
        return err;
    }

    err = graph_draw(*inv_fg_diff_fn, std::pair(-0.1, 0.1), STD_STEP / 10, false, "Inv Funcions difference");
    if (err) {
        print_errs(err);
        return err;
    }

    std::cout << "(f - g)(" << (*inv_fg_diff_fn)(0) << ") = 0" << std::endl;
    std::cout << "f(" << (*inv_fg_diff_fn)(0) << ") = " << (*f)((*inv_fg_diff_fn)(0)) << std::endl;
    std::cout << "g(" << (*inv_fg_diff_fn)(0) << ") = " << (*g)((*inv_fg_diff_fn)(0)) << std::endl;
    return err;
}

template <typename T>
std::vector<T> operator+(const std::vector<T>& fst, const std::vector<T>& scd) {
    std::vector<double> sum;
    sum.reserve(fst.size() + scd.size());

    std::copy(fst.begin(), fst.end(), std::back_inserter(sum));
    std::copy(scd.begin(), scd.end(), std::back_inserter(sum));

    return sum;
}
