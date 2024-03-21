#ifndef __POINT_TABLE_HPP__
#define __POINT_TABLE_HPP__

#include "errs.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <optional>
#include <functional>

struct Point {
    double x;
    double y;

    friend std::ostream& operator<<(std::ostream& os, const Point& data);
};

class PointsTable {
    errs_t err = OK;
    std::vector<Point> data;

public:
    explicit PointsTable(void) : data({}) {}
    explicit PointsTable(std::ifstream& file);
    explicit PointsTable(const std::initializer_list<Point>& points) : data(points) {}

    void sort(void);
    std::size_t size(void) const;
    errs_t is_incorrect(void) const;

    double min_step(void);
    PointsTable point_range(double x, std::size_t len) const;
    PointsTable slice(std::size_t start, std::size_t end) const;

    auto end(void) const { return data.end(); }
    auto begin(void) const { return data.begin(); }

    void push_back(Point point);
    Point operator[](std::size_t index) const;

    std::vector<double> get_ys(void) const;
    std::vector<double> get_xs(void) const;

    friend std::optional<std::function<double(double)>> newton_polinom(const PointsTable &table, int *new_n, int n);
    friend std::optional<std::function<double(double)>> hermit_polinom(const PointsTable &table, int *new_n, int n);

    friend std::ostream& operator<<(std::ostream& os, const PointsTable& data);
};

#endif // __POINT_TABLE_HPP__
