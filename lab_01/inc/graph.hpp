#ifndef __GRAHP_HPP__
#define __GRAHP_HPP__

#include <iostream>
#include <functional>

#include "errs.h"

errs_t graph_draw(std::function<double(double)>, std::pair<double, double>, double, bool = true, std::string = "Unknown func");
errs_t graphs_draw(std::pair<std::function<double(double)>, std::function<double(double)>>, std::pair<double, double>, double, bool = true);

#endif // __GRAHP_HPP__