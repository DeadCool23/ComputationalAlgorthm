#include "graph.hpp"
#include <fstream>
#include <iostream>

static void proc_end(void) {
    char is_exit = 0;
    std::cout << "<Press any key to exit>";

    while (!is_exit)
        std::cin >> is_exit;
}

errs_t graphs_draw(std::vector<PointsTable> tables, std::vector<std::string> lbls, bool del) {
    std::vector<std::string> data_files;
    for (std::size_t i = 0; i < tables.size(); ++i) {
        data_files.push_back("./misc/" + lbls[i] + ".csv");
        std::ofstream file(data_files[i]);
        if (!file.is_open()) return ERR_FILE;
        for (std::size_t j = 0; j < tables[i].size(); ++j)
            file << tables[i][j].x << " " << tables[i][j].y << std::endl;
        file.close();
    }

    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe)
        return ERR_FUNC;

    fprintf(gnuplotPipe, "set title 'funcs'\n");

    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");

    for (std::size_t i = 0; i < data_files.size(); ++i) {
        fprintf(gnuplotPipe, !i ? "plot '" : ", '");
        fprintf(gnuplotPipe, (data_files[i] + "' title '" + lbls[i] + "' w l").c_str());
    }
    fprintf(gnuplotPipe, "\n");
    fflush(gnuplotPipe);

    proc_end();

    pclose(gnuplotPipe);
    for (std::size_t i = 0; del && i < data_files.size(); ++i)
        std::remove(data_files[i].c_str());
    return OK;
}