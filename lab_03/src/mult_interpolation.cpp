#include "mult_interpolation.hpp"

#include "newton.hpp"
#include "splines.hpp"
#include "point_table.hpp"

static double mult_NEWTON(const Points4D &table, const Point3D &point, const std::vector<size_t> &ndegs) {
    if (ndegs.size() != 3)
        throw std::invalid_argument("Number of intervals must be 3");
    
    PointsTable zs;
    for (size_t iz = 0; iz < table._z.size(); ++iz) {
        PointsTable ys;
        for (size_t iy = 0; iy < table._y.size(); ++iy) {
            PointsTable xs;
            for (size_t ix = 0; ix < table._x.size(); ++ix) {
                xs.push_back({table._x[ix], table._phi[iz][iy][ix]});
            }
            ys.push_back({table._y[iy], NewtonPolinom(xs.point_range(point.x, ndegs[0] + 1))(point.x)});
        }
        zs.push_back({table._z[iz], NewtonPolinom(ys.point_range(point.y, ndegs[1] + 1))(point.y)});
    }
    return NewtonPolinom(zs.point_range(point.z, ndegs[2] + 1))(point.z);
}

static double mult_SPLINE(const Points4D &table, const Point3D &point) {
    PointsTable zs;
    for (size_t iz = 0; iz < table._z.size(); ++iz) {
        PointsTable ys;
        for (size_t iy = 0; iy < table._y.size(); ++iy) {
            PointsTable xs;
            for (size_t ix = 0; ix < table._x.size(); ++ix) {
                xs.push_back({table._x[ix], table._phi[iz][iy][ix]});
            }
            ys.push_back({table._y[iy], Splines(xs)(point.x)});
        }
        zs.push_back({table._z[iz], Splines(ys)(point.y)});
    }
    return Splines(zs)(point.z);
}

#define RAND_CHOOSE_INTERPOL(table, point, deg) \
    (rand() % 2 \
        ? NewtonPolinom(table.point_range(point, deg + 1))(point) \
        : Splines(table)(point)) 

static double mult_MIXED(const Points4D &table, const Point3D &point, const std::vector<size_t> &ndegs) {
    srand(time(NULL));
    if (ndegs.size() != 3)
        throw std::invalid_argument("Number of intervals must be 3");

    PointsTable zs;
    for (size_t iz = 0; iz < table._z.size(); ++iz) {
        PointsTable ys;
        for (size_t iy = 0; iy < table._y.size(); ++iy) {
            PointsTable xs;
            for (size_t ix = 0; ix < table._x.size(); ++ix) {
                xs.push_back({table._x[ix], table._phi[iz][iy][ix]});
            }
            ys.push_back({table._y[iy], RAND_CHOOSE_INTERPOL(xs, point.x, ndegs[0])});
        }
        zs.push_back({table._y[iz], RAND_CHOOSE_INTERPOL(ys, point.y, ndegs[1])});
    }
    return RAND_CHOOSE_INTERPOL(zs, point.z, ndegs[2]);
}

double mult_interpolation(const Points4D &table, const Point3D &point, 
    const std::vector<size_t> &ndegs, interpol_type_t type) {
    return type == SPLINE
        ? mult_SPLINE(table, point)
        : type == NEWTON
            ? mult_NEWTON(table, point, ndegs)
            : mult_MIXED(table, point, ndegs);
}