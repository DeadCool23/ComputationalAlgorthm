#include "errs.H"

#include <fstream>
#include <iostream>
#include <functional>

void write_table(std::ofstream &file, std::function<double(double)> func, std::pair<double, double> dist = std::pair(0, 1), double step = 0.25);

int main(int argc, char **argv) {
    if (argc != 2)
        return ERR_FILE;

    std::ofstream file(argv[1]);
    write_table(file, [](double x) { return x * x - 1.5; });
    return OK;    
}


void write_table(std::ofstream &file, std::function<double(double)> func, std::pair<double, double> dist, double step) {
    for (double point = dist.first; point <= dist.second; point += step)
        file << point << " " << func(point) << std::endl;
}
