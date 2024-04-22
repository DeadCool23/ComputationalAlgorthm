#ifndef __MULT_INTER_HPP__
#define __MULT_INTER_HPP__

#include "points_4d.hpp"

typedef enum {
    NEWTON,
    SPLINE,
    MIXED
} interpol_type_t;

double mult_interpolation(const Points4D &table, const Point3D &point, const std::vector<size_t> &ndegs, interpol_type_t type);

#endif // __MULT_INTER_HPP__