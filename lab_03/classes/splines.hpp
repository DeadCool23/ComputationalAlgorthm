#ifndef __SPLINES_HPP__
#define __SPLINES_HPP__

#include <vector>
#include <fstream>

#include "point_table.hpp"

struct Spline {
    double a, b, c, d;

    // 1. a, 1. c, 2. [b, d]
    void a_calc(double y);
    void b_calc(std::pair<double, double> ys, std::pair<double, double> cs, double h);
    void c_calc(double next_c, double ksi, double teta) ;
    void d_calc(std::pair<double, double> cs, double h);

    double operator()(double x, double x0 = 0) const;
    friend std::ostream& operator<<(std::ostream& os, const Spline& func);
};

class Splines {
    std::size_t cnt;
    std::vector<double> _x;
    std::vector<Spline> splines;

public:
    Splines(std::ifstream &file, std::pair<double, double> bords = std::pair(0 ,0));
    Splines(const PointsTable  &table, std::pair<double, double> bords = std::pair(0 ,0));

    double operator()(double x) const;
    friend std::ostream& operator<<(std::ostream& os, const Splines& splines);
private:
    void a_coefs_calc(std::vector<double> _y);
    void b_coefs_calc(const PointsTable &table, std::pair<double, double> bords);
    void c_coefs_calc(const PointsTable &table, std::pair<double, double> bords);
    void d_coefs_calc(std::pair<double, double> bords);

    void splines_calc(const PointsTable &table, std::pair<double, double> bords);
};

#endif // __SPLINES_HPP__
