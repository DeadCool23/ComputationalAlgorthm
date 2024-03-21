#ifndef __NEWTON_HPP__
#define __NEWTON_HPP__

#include <vector>
#include <fstream>
#include <functional>

#include "errs.H"
#include "point_table.hpp"

class NewtonPolinom {
private:
    errs_t err;
    std::vector<double> _x;
    std::vector<double> coefs;

    std::size_t d_degree = 0;
    std::function<double(double, std::size_t, std::vector<std::size_t>)> term_calc;
public:
    explicit NewtonPolinom(std::ifstream &file);
    explicit NewtonPolinom(const PointsTable table);

    NewtonPolinom diff(void) const;
    std::size_t diff_degree(void) const;

    double operator()(double x) const;

    errs_t is_incorrect(void) const;
    friend std::ostream& operator<<(std::ostream& os, const NewtonPolinom &polinom);
private:
    void divided_differences(const PointsTable &table);
    std::function<double(double, std::size_t, std::vector<std::size_t>)> term_calc_init(void);
};

#endif // __NEWTON_HPP__