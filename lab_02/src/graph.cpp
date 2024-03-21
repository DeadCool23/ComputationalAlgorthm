#include "graph.hpp"
#include <fstream>
#include <iostream>

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

    pclose(gnuplotPipe);
    for (std::size_t i = 0; del && i < data_files.size(); ++i)
        std::remove(data_files[i].c_str());
    return OK;
}

errs_t graph_draw(PointsTable table, std::string label, bool del) {
    std::string data_file = "./misc/" + label + ".csv";
    std::ofstream file(data_file);
    if (!file.is_open()) return ERR_FILE;
    for (std::size_t i = 0; i < table.size(); ++i)
        file << table[i].x << " " << table[i].y << std::endl;
    file.close();

    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe)
        return ERR_FUNC;

    fprintf(gnuplotPipe, "set title 'funcs'\n");

    fprintf(gnuplotPipe, "set xlabel 'x'\n");
    fprintf(gnuplotPipe, "set ylabel 'y'\n");

    fprintf(gnuplotPipe, "plot '");
    fprintf(gnuplotPipe, (data_file + "' title '" + label + "' w l").c_str());

    fprintf(gnuplotPipe, "\n");
    fflush(gnuplotPipe);

    pclose(gnuplotPipe);
    if (del)
        std::remove(data_file.c_str());
    return OK;
}
