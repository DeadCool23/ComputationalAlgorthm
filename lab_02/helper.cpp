#include "errs.H"

#include <math.h>
#include <fstream>
#include <iostream>
#include <functional>

std::vector<std::function<double(double)>> funcs = {
    [](double x) { return x; },
    [](double x) { return x * x; },
    [](double x) { return x * x * x; },
    [](double x) { return sin(x); },
    [](double x) { return cos(x); },
    [](double x) { return log(x); },
};

void write_table(std::ofstream &file, std::function<double(double)> func, std::pair<double, double> dist = std::pair(-10, 10), double step = 1);

int main(int argc, char **argv) {
    if (argc != 2)
        return ERR_FILE;

    std::ofstream file(argv[1]);
    write_table(file, funcs[3]);
    return OK;    
}


void write_table(std::ofstream &file, std::function<double(double)> func, std::pair<double, double> dist, double step) {
    for (double point = dist.first; point <= dist.second; point += step)
        file << point << " " << func(point) << std::endl;
}
