#ifndef __POINTS_4D_HPP__
#define __POINTS_4D_HPP__

#define N 3

#include <vector>
#include <functional>
#include "funcs_gen.hpp"
extern "C" {
    #include <stdlib.h>
}

struct Point3D {
    double x;
    double y;
    double z;
};

struct Points4D {
    public:
        std::vector<double> _x;
        std::vector<double> _y;
        std::vector<double> _z;
        std::vector<std::vector<std::vector<double>>> _phi;
    
    public:
        Points4D(void) : _x(), _y(), _z(), _phi() {};
        Points4D(const std::vector<std::pair<Interval, double>> &intevals, const std::function<double(double, double, double)> &func);

        void print_table(void) const;
    
    private:
        void print_head(void) const;
};

#endif // __POINTS_4D_HPP__
