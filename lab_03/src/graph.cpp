#include "graph.hpp"
#include "mult_interpolation.hpp"

#include <fstream>
#include <iostream>

#pragma GCC diagnostic ignored "-Wformat-security"

#define NEWTON_3_DEGREE {3, 3, 3}

errs_t graph_draw(const Points4D &table, const std::vector<size_t> &ranges, std::string label, bool del) {
#define EPS 1e-8
    if (ranges.size() != 3) throw std::invalid_argument("Number of intervals must be 3");
    std::string data_file = "./misc/" + label + ".dat";
    std::ofstream file(data_file);
    if (!file.is_open()) return ERR_FILE;

    auto x_step = (table._x[table._x.size() - 1] - table._x[0]) / (ranges[0] - 1);
    auto y_step = (table._y[table._y.size() - 1] - table._y[0]) / (ranges[1] - 1);
    auto z_step = (table._z[table._z.size() - 1] - table._z[0]) / (ranges[2] - 1);

    for (double ix = table._x[0]; ix <= table._x[table._x.size() - 1] + EPS; ix += x_step) {
        for (double iy = table._y[0]; iy <= table._y[table._y.size() - 1] + EPS; iy += y_step) {
            for (double iz = table._z[0]; iz <= table._z[table._z.size() - 1] + EPS; iz += z_step) {
                file << ix << " " << iy  << " " << iz << " "
                     << mult_interpolation(table, {ix, iy, iz}, NEWTON_3_DEGREE, NEWTON) 
                     << std::endl;
            }
        }
    }
    file.close();

    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe)
        return ERR_FUNC;

    fprintf(gnuplotPipe, "set title 'func 4D'\n");

    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");
    fprintf(gnuplotPipe, "set zlabel 'z'\n");

    fprintf(gnuplotPipe, "splot '");
    fprintf(gnuplotPipe, (data_file + "' title '" + label).c_str());
    fprintf(gnuplotPipe, "' w p lt palette pointsize 2 pointtype 7");

    fprintf(gnuplotPipe, "\n");
    fflush(gnuplotPipe);

    pclose(gnuplotPipe);
    if (del)
        std::remove(data_file.c_str());
    return OK;
#undef EPS
}
