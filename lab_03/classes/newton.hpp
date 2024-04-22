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

public:
    explicit NewtonPolinom(std::ifstream &file);
    explicit NewtonPolinom(const PointsTable table);

    double operator()(double x) const;

    errs_t is_incorrect(void) const;
    friend std::ostream& operator<<(std::ostream& os, const NewtonPolinom &polinom);
private:
    void divided_differences(const PointsTable &table);
};

#endif // __NEWTON_HPP__