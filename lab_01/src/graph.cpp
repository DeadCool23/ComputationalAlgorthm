#include "graph.hpp"

#include <fstream>

errs_t graph_draw(std::function<double(double)> func, std::pair<double, double> dist, double step, bool del, std::string funcname) {
    std::string data_file = "misc/" + funcname + ".csv";
    std::ofstream dataFile(data_file.c_str());
    if (!dataFile.is_open())
        return ERR_FILE;

    for (double x = dist.first; x <= dist.second; x += step)
        dataFile << x << " " << func(x) << std::endl;
    dataFile.close();

    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe)
        return ERR_FUNC;

    fprintf(gnuplotPipe, "set title '"); fprintf(gnuplotPipe, funcname.c_str()); fprintf(gnuplotPipe, "'\n");

    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");

    fprintf(gnuplotPipe, "plot '"); fprintf(gnuplotPipe, data_file.c_str()); fprintf(gnuplotPipe, "' title 'f(x)' w lp\n");
    fflush(gnuplotPipe);
    
    pclose(gnuplotPipe);
    int res = del ? std::remove(data_file.c_str()) : 0;
    return res ? ERR_CALC : OK;
}

errs_t graphs_draw(std::pair<std::function<double(double)>, std::function<double(double)>> funcs, std::pair<double, double> dist, double step, bool del) {
    std::pair<std::string, std::string> data_files = std::pair("misc/fst.csv", "misc/scd.csv");
    std::pair<std::ofstream, std::ofstream> dataFiles = std::pair(std::ofstream(data_files.first.c_str()), std::ofstream(data_files.second.c_str()));
    if (!dataFiles.first.is_open() || !dataFiles.second.is_open())
        return ERR_FILE;

    for (double x = dist.first; x <= dist.second; x += step) {
        dataFiles.first << x << " " << funcs.first(x) << std::endl;
        dataFiles.second << x << " " << funcs.second(x) << std::endl;
    }
    dataFiles.first.close();
    dataFiles.second.close();

    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe)
        return ERR_FUNC;

    fprintf(gnuplotPipe, "set title 'funs'\n");

    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");

    fprintf(gnuplotPipe, "plot '"); fprintf(gnuplotPipe, data_files.first.c_str()); fprintf(gnuplotPipe, "' title 'f(x)' w lp");
    fprintf(gnuplotPipe, ", '"); fprintf(gnuplotPipe, data_files.second.c_str()); fprintf(gnuplotPipe, "' title 'g(x)' w lp\n");
    fflush(gnuplotPipe);
    
    pclose(gnuplotPipe);
    int res = del ? std::remove(data_files.first.c_str()) : 0;
    res = del ? (!res ? std::remove(data_files.second.c_str()) : res) : 0;
    return res ? ERR_CALC : OK;
}
