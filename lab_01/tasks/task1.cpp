#include <cmath>
#include <limits>
#include <fstream>
#include <iostream>

#include "errs.h"
#include "interpol.hpp"
#include "point_table.hpp"

const std::string BLUE = "\033[34m";
const std::string RESET = "\033[0m";

#define x 0.54
#define MAX_POLINOM_DEGREE 10

int main(int argc, char **argv) {
    errs_t err = OK;
    std::ifstream file(argc < 2 ? "data/std_data.txt" : argv[1]);
    if (!file.is_open()) {
        print_errs(err = ERR_FILE);
        return err;
    }
    PointsTable data = PointsTable(file);
    file.close();
    
    if ((err = data.is_incorrect()) != OK)
        print_errs(err);
    
    int polinom_n;
    for (std::size_t i = 0; i <= MAX_POLINOM_DEGREE && !err; i++) {
        int polinoms_n[2];
        PointsTable newton_close_data, hermit_close_data;
        if (i) {
            newton_close_data = data.point_range(x, i);
            hermit_close_data = data.point_range(x, i, true);
        }
        auto newton = newton_polinom(i ? newton_close_data : data, i ? &polinom_n : polinoms_n, i ? i : -1);
        auto hermit = hermit_polinom(i ? hermit_close_data : data, i ? &polinom_n : polinoms_n + 1, i ? i : -1);

        auto newton_x = newton ? (*newton)(x) : std::numeric_limits<double>::infinity();
        auto hermit_x = hermit ? (*hermit)(x) : std::numeric_limits<double>::infinity();

        if (!i) std::cout << BLUE << "====Full traversal====" << RESET << std::endl;

        std::cout << "P" << (i ? polinom_n : polinoms_n[0]) << "(" << x << ")" << " = " << newton_x << std::endl;
        std::cout << "H" << (i ? polinom_n : polinoms_n[1]) << "(" << x << ")" << " = " << hermit_x << std::endl;
        std::cout << "diff: " << fabs(newton_x - hermit_x) << std::endl;
        std::cout << (i ? "======================" : (BLUE + "====Cmp by degrees====" + RESET)) << std::endl;
    }
    return err;
}
