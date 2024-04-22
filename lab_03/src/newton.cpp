#include "newton.hpp"
#include <iostream>

extern "C" {
    #include "stdlib.h"
}

NewtonPolinom::NewtonPolinom(const PointsTable table) {
    errs_t _err = OK;
    if ((_err = table.is_incorrect()))
        err = _err;
    else
        divided_differences(table);
}
NewtonPolinom::NewtonPolinom(std::ifstream &file) { 
    divided_differences(PointsTable(file)); 
}

double NewtonPolinom::operator()(double x) const {
    double term = 1;
    double result = 0;
    std::size_t n = coefs.size();
    for (std::size_t i = 0; i < n; i++) {
        result += coefs[i] * term;
        term *= (x - _x[i]);
    }
    return result;
}

errs_t NewtonPolinom::is_incorrect(void) const { return err; }
std::ostream& operator<<(std::ostream& os, const NewtonPolinom &polinom) {
    os << "Xi: ";
    for ( auto x : polinom._x )
        os << x << ", ";
    os << std::endl;

    os << "Coefs: ";
    for ( auto c : polinom.coefs )
        os << c << ", ";
    os << std::endl;
    return os;
}

void NewtonPolinom::divided_differences(const PointsTable &table) {
    auto y_data = table.get_ys();
    std::vector<double> result = { table[0].y };

    std::size_t k = 1, n = table.size() - 1;
    for (; y_data.size() != 1 && k <= n; k++) {
        std::vector<double> data;
        for (std::size_t i = 0; i < y_data.size() - 1; i++) {
            if (table[i].x == table[i + k].x)
                goto error;

            data.push_back((y_data[i] - y_data[i + 1]) / (table[i].x - table[i + k].x));
        }
        result.push_back((y_data = data)[0]);
    }
    error: err = ERR_CALC;
    _x = table.get_xs();
    coefs = err ? result : std::vector<double>();
}
