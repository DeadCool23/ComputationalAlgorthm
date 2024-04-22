#include "points_4d.hpp"

#include <iomanip>
#include <iostream>

#define FORMAT(size, nums_after_com) std::setw(size) << std::setprecision(nums_after_com) << std::fixed << std::internal

Points4D::Points4D(const std::vector<std::pair<Interval, double>> &intevals, const std::function<double(double, double, double)> &func) {
    if (intevals.size() != 3) throw std::invalid_argument("Number of intervals must be 3");
    auto x_step = (intevals[0].first.end - intevals[0].first.begin) / (intevals[0].second - 1);
    auto y_step = (intevals[1].first.end - intevals[1].first.begin) / (intevals[1].second - 1);
    auto z_step = (intevals[2].first.end - intevals[2].first.begin) / (intevals[2].second - 1);

    size_t iter_y = 0, iter_z = 0;
    for (double iz = intevals[2].first.begin; iz <= intevals[2].first.end; iz += z_step) {
        iter_y = 0;
        _z.push_back(iz); 
        _phi.push_back(std::vector<std::vector<double>>());
        for (double iy = intevals[1].first.begin; iy <= intevals[1].first.end; iy += y_step) {
            if (iz == intevals[2].first.begin) _y.push_back(iy);
            
            _phi[iter_z].push_back(std::vector<double>());
            for (double ix = intevals[0].first.begin; ix <= intevals[0].first.end; ix += x_step) {
                if (iz == intevals[2].first.begin && iy == intevals[1].first.begin)
                    _x.push_back(ix);

                _phi[iter_z][iter_y].push_back(func(ix, iy, iz));

            }
            iter_y++;
        }
        iter_z++;
    }
}

#include "stdio.h"
void Points4D::print_head(void) const {
    std::cout << " y/x |";
    for (size_t i = 0; i < _x.size(); ++i)
        std::cout << " " << FORMAT(10, 3) << _x[i] << " |";
    std::cout << std::endl;
}

void Points4D::print_table(void) const {
    for (size_t iz = 0; iz < _z.size(); ++iz) {
        std::cout << std::endl << std::endl;
        std::cout << "z = " << FORMAT(0, 3) << _z[iz];
        std::cout << std::endl;
        this->print_head();
        for (size_t iy = 0; iy < _y.size(); ++iy) {
            std::cout << FORMAT(4, 1) << _y[iy] << " |";
            for (size_t ix = 0; ix < _x.size(); ++ix) {
                std::cout << " " << FORMAT(10, 3) << _phi[iz][iy][ix] << " |";
            }
            printf("\n");
        }
    }
}