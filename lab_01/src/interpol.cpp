#include "interpol.hpp"

#include <math.h>
#include <vector>

typedef enum {
    NEWTON,
    HERMIT
} polinom_type;

#define EPS 1e-10
double factorial(double n) { return fabs(n) < EPS ? 1 : n * factorial(n - 1); }
#undef EPS

static std::vector<double> divided_differences(const PointsTable &table, std::size_t n, errs_t &err, polinom_type type) {
    auto y_data = table.get_ys();
    std::vector<double> result = { table[0].y };

    std::size_t k = 1;
    for (; y_data.size() != 1 && k <= n; k++) {
        std::vector<double> data;
        for (std::size_t i = 0; i < y_data.size() - 1; i++) {
            if (table[i].x == table[i + k].x && (type == NEWTON || (type == HERMIT && table[i].diffs() < k)))
                goto error;

            if (type == NEWTON) {
                data.push_back((y_data[i] - y_data[i + 1]) / (table[i].x - table[i + k].x));
            } else if (type == HERMIT) {
                data.push_back(
                    (table[i].x == table[i + k].x)
                        ? (1 / factorial(k)) * (table[i].dy[k - 1])
                        : (y_data[i] - y_data[i + 1]) / (table[i].x - table[i + k].x)
                );
            }
        }
        result.push_back((y_data = data)[0]);
    }
    return result;

    error: err = ERR_CALC;
    return std::vector<double>();
}

static std::function<double(double)> build_polinom(std::vector<double> &coefs,const PointsTable &table, std::size_t n) {
    return [=](double x) {
        double term = 1;
        double result = 0;

        for (std::size_t i = 0; i <= n; i++) {
            result += coefs[i] * term;
            term *= (x - table[i].x);
        }

        return result;
    };
}

std::optional<std::function<double(double)>> newton_polinom(const PointsTable &table, int *new_n, int n) {
    n = n == -1 ? table.size() - 1
                : (int)table.size() > n ? n 
                                        : table.size() - 1;

    errs_t err = OK;
    if (new_n) *new_n = n;
    auto coefs = divided_differences(table, n, err, NEWTON);
    return !err ? std::optional(build_polinom(coefs, table, n)) : std::nullopt;
}

static PointsTable prepare_to_hermit(const PointsTable &table) {
    PointsTable hermit_table;
    for (std::size_t i = 0; i < table.size(); i++)
        for (std::size_t j = 0; j <= table[i].diffs(); j++) 
            hermit_table.push_back(table[i]);
    
    return hermit_table;
} 

std::optional<std::function<double(double)>> hermit_polinom(const PointsTable &table, int *new_n, int n) {
    auto new_table = prepare_to_hermit(table);
    n = n == -1 ? new_table.size() - 1
                : (int)new_table.size() > n ? n 
                                            : table.size() - 1;
    
    errs_t err = OK;
    if (new_n) *new_n = n;
    auto coefs = divided_differences(new_table, n, err, HERMIT);
    return !err ? std::optional(build_polinom(coefs, new_table, n)) : std::nullopt;
}