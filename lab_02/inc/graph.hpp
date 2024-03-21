#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include "errs.H"
#include "point_table.hpp"

#include <vector>

errs_t graph_draw(PointsTable table, std::string label, bool del = true);
errs_t graphs_draw(std::vector<PointsTable> tables, std::vector<std::string> lbls, bool del = true);

#endif /* __GRAPH_HPP__ */