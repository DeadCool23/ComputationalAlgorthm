#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include "errs.H"
#include "points_4d.hpp"

#include <string>
#include <vector>

errs_t graph_draw(const Points4D &table, const std::vector<size_t> &ranges, std::string label, bool del = true);

#endif /* __GRAPH_HPP__ */