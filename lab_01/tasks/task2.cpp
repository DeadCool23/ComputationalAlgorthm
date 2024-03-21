#include <cmath>
#include <cstdio>
#include <limits>
#include <fstream>
#include <iostream>
#include <functional>

#include "errs.h"
#include "graph.hpp"
#include "interpol.hpp"
#include "point_table.hpp"

#define STD_STEP 0.2
#define STD_DIST std::pair(0, 3)

static double bin_finder_of_root(const PointsTable &table);
static double bin_finder_of_root_abstract(const PointsTable &table);
auto passer = [](double x) { return (x * 0) + std::numeric_limits<double>::infinity(); };

int main(int argc, char **argv) {
    errs_t err = OK;
    std::ifstream file(argc < 2 ? "data/std_data.txt" : argv[1]);
    if (!file.is_open()) {
        print_errs(err = ERR_FILE);
        return err;
    }

    PointsTable data = PointsTable(file);
    PointsTable inv_data = data.reverse_func(); //.point_range(0, 15);
    file.close();
    
    auto bin_root_finder = rand() % 2 ? bin_finder_of_root : bin_finder_of_root_abstract;
    double newton_root = newton_polinom(inv_data, nullptr, 4).value_or(passer)(0);
    double hermit_root = inv_data.is_incorrect() 
        ? bin_root_finder(data)
        : hermit_polinom(inv_data, nullptr, 4).value_or(passer)(0);

    std::cout << "Function root:" << std::endl;
    std::cout << "f(" << newton_root << ") = 0" << " by Newton" << std::endl;
    std::cout << "f(" << hermit_root << ") = 0" << " by Hermit" << std::endl;

    auto newton_fn = newton_polinom(data);
    auto hermit_fn = hermit_polinom(data);
    if (newton_fn && hermit_fn) {
        err = graph_draw(*newton_fn, STD_DIST, STD_STEP, false, "Newton polinom");
        err = err ? err : graph_draw(*hermit_fn, STD_DIST, STD_STEP, false, "Hermit polinom");
    } else {
        std::cout << "Can't build polinom";
        err = ERR_CALC;
    }
    print_errs(err);
    return err;
}

static double calc_ans(const PointsTable &table, double x) {
    return newton_polinom(table.point_range(x, 3)).value_or(passer)(x);
}

static double bin_finder_of_root(const PointsTable &table) {
#define EPS 1e-10
#define TRIES_CNT 1e+10
    Point closest_point = table[0];
    double step_center = fabs(table[0].x - table[table.size() - 1].x) / 2;
    std::cout << step_center << std::endl;

    auto is_root = [](const Point &point) -> bool {
        return fabs(point.y) < EPS;
    };
    for (std::size_t i = 0; !is_root(closest_point) && i < table.size() - 1; ++i) {
        if (table[i].y * table[i + 1].y < 0) {
            closest_point = table[i];
            step_center = fabs(table[i + 1].x - closest_point.x) / 2;
        }
    }
    std::cout << step_center << std::endl;

    std::size_t tries = 0;
    double step = step_center / 2;
    std::cout << closest_point << std::endl << std::endl;
    auto finder_of_clpoint = closest_point;
    while (!is_root(finder_of_clpoint)) {
        std::cout << finder_of_clpoint << " S: " << step_center << std::endl;
        if (tries++ == TRIES_CNT) goto time_error;

        auto new_point_x = closest_point.x + step_center;
        auto new_point_y = calc_ans(table, new_point_x);

        Point tmp_point = {
            .x = new_point_x,
            .y = new_point_y,
            .dy = std::vector<double>()
        };

        std::cout << "Tmp: " << tmp_point << std::endl;
        step_center += tmp_point.y > 0 ? -step : step;
        finder_of_clpoint = tmp_point;
        step /= 2;
    }

    std::cout << tries << std::endl;
    return finder_of_clpoint.x;
    time_error: return std::numeric_limits<double>::infinity();
#undef TRIES_CNT
#undef EPS
}

static double bin_finder_of_root_abstract(const PointsTable &table) {
#define EPS 1e-10
#define TRIES_CNT 1e+5
    double step;
    Point closest_point;
    closest_point = table.closest_to_zero_y();

    auto del_point_from_table = [](const PointsTable &data, Point dpoint) -> PointsTable {
        PointsTable ntable;
        for ( auto point : data ) {
            if (point != dpoint)
                ntable.push_back(point);
        }
        return ntable;
    };

    auto nctable = del_point_from_table(table, closest_point);
    auto less_closest_point = nctable.closest_to_zero_y();

    step = fabs(less_closest_point.x - closest_point.x) / 2;

    std::size_t tries = 0;
    while (fabs(closest_point.y) > EPS) {
        if (tries++ == TRIES_CNT) goto time_error;

        auto new_point_xs = std::pair(closest_point.x + step, closest_point.x - step);
        auto new_point_ys = std::pair(calc_ans(table, new_point_xs.first), 
                                      calc_ans(table, new_point_xs.second));

        bool chooser = fabs(new_point_ys.first) < fabs(new_point_ys.second);
        Point tmp_point = {
            .x = chooser ? new_point_xs.first : new_point_xs.second,
            .y = chooser ? new_point_ys.first : new_point_ys.second,
            .dy = std::vector<double>()
        };

        bool to_save = fabs(tmp_point.y) > fabs(closest_point.y);
        step = to_save ? step / 2 : step; 
        closest_point = to_save ? closest_point : tmp_point;
    }

    return closest_point.x;
    time_error: return std::numeric_limits<double>::infinity();
#undef TRIES_CNT
#undef EPS
}