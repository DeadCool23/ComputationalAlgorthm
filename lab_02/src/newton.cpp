#include "newton.hpp"
#include <iostream>

extern "C" {
    #include "stdlib.h"
}

int factorial(int n) { return !abs(n) ? 1 : n * factorial(n - 1); }

std::function<double(double, std::size_t, std::vector<std::size_t>)> NewtonPolinom::term_calc_init(void) {
    auto __x = _x;
    auto in_vec = [](std::vector<std::size_t> vec, std::size_t i) -> bool {
        for (auto _i : vec)
            if (_i == i)
                return true;
        return false;
    };

    return [__x, in_vec](double x, std::size_t n, std::vector<std::size_t> ignore_index) -> double {
        double term = 1;
        for (std::size_t i = 0; i < n; ++i)
            if (!ignore_index.size() || !in_vec(ignore_index, i))
                term *= (x - __x[i]);
        return term;
    };
}

NewtonPolinom::NewtonPolinom(const PointsTable table) {
    errs_t _err = OK;
    if ((_err = table.is_incorrect()))
        err = _err;
    else {
        divided_differences(table);
        term_calc = term_calc_init();
    }
}
NewtonPolinom::NewtonPolinom(std::ifstream &file) { 
    divided_differences(PointsTable(file)); 
    term_calc = term_calc_init();
}

/*
 * Pn = c + c1 (x - x_0) + c2 (x - x_0) (x - x_1) + c3 (x - x_0) (x - x_1) (x - x_2)
 * Pn' = c1 + c2 ((x - x_0) + (x - x_1)) + c3 ((x - x_0) (x - x_1) + (x - x_0) (x - x_2) + (x - x_1) (x - x_2))
 * Pn'' = 2 * c2 + 2 * c3 ((x - x_0) + (x - x_1) + (x - x_2))
 * Pn'' = 2 * c2 + 2 * c3 (3 * x - (x_0 + x_1 + x_2))
 */
NewtonPolinom NewtonPolinom::diff(void) const {
    NewtonPolinom _new = *this;
    ++_new.d_degree;

    auto __x = _x;
    auto prev_term = term_calc;
    std::size_t _prev_d = this->d_degree;

    auto in_vec = [](std::vector<std::size_t> vec, std::size_t i) -> bool {
        for (auto _i : vec)
            if (_i == i)
                return true;
        return false;
    };

    _new.term_calc = [__x, prev_term, _prev_d, in_vec](double x, std::size_t n, std::vector<std::size_t> ignore_index) -> double {
        double sum = 0;
        auto new_ignore = ignore_index;
        new_ignore.resize(ignore_index.size() + 1);
        for (std::size_t i = 0; i < n; ++i) {
            if (!ignore_index.size() || !in_vec(ignore_index, i)) {
                new_ignore[new_ignore.size() - 1] = i;
                sum += prev_term(x, n, new_ignore);
            }
        }
        return sum;
    };
    return _new;
};
std::size_t NewtonPolinom::diff_degree(void) const { return d_degree; };

double NewtonPolinom::operator()(double x) const {
    double result = 0;
    std::size_t n = coefs.size();
    double term = factorial(d_degree);
    for (std::size_t i = d_degree; i < n; i++) {
        result += coefs[i] * term;
        term = term_calc(x, i + 1, std::vector<std::size_t>());
    }

    return result;
}

errs_t NewtonPolinom::is_incorrect(void) const { return err; }
std::ostream& operator<<(std::ostream& os, const NewtonPolinom &polinom) {
    os << "Diff degree: " << polinom.d_degree << std::endl;
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