#ifndef __INTERPOL_HPP__
#define __INTERPOL_HPP__

#include <optional>
#include <functional>

#include "point_table.hpp"

std::optional<std::function<double(double)>> newton_polinom(const PointsTable &table, int *new_n = nullptr, int n = -1);
std::optional<std::function<double(double)>> hermit_polinom(const PointsTable &table, int *new_n = nullptr, int n = -1);

#endif // __INTERPOL_HPP__