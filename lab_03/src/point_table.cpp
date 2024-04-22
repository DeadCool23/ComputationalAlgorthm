#include "point_table.hpp"

#include <cmath>
#include <algorithm>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Point& data) {
    std::string diff = "";
    os << "x:" << data.x << " y:" << data.y;
    return os;
}

PointsTable::PointsTable(std::ifstream& file) {
    if (!file.is_open())
        err = ERR_FILE;

    std::string line;
    while (std::getline(file, line) && !err) {
        Point tmp_data;
        std::istringstream iss(line);
        if (!(iss >> tmp_data.x >> tmp_data.y))
            err = ERR_IO;
        data.push_back(tmp_data);
    }
    if (data.size() <= 1)
        err = ERR_FILE;
    
    this->sort();
    if (!file.eof())
        err = ERR_IO;
    file.clear();
    file.seekg(0, std::ios::beg);
}

std::vector<double> PointsTable::get_ys(void) const {
    std::vector<double> y_data;
    for (std::size_t i = 0; i < (this->data).size(); i++)
        y_data.push_back((this->data)[i].y);
    return y_data;
}

std::vector<double> PointsTable::get_xs(void) const {
    std::vector<double> x_data;
    for (std::size_t i = 0; i < data.size(); i++)
        x_data.push_back(data[i].x);
    return x_data;
}

double PointsTable::min_step(void) {
    double min_step = 0;
    if (data.size() <= 1) {
        err = ERR_CALC;
    } else {
        min_step = data[1].x - data[0].x;
        for (std::size_t i = 1; i < data.size() - 1; ++i) {
            double new_step = data[i + 1].x - data[i].x;
            if (new_step < min_step)
                min_step = new_step;
        }
    }
    return min_step;
}

PointsTable PointsTable::slice(std::size_t start, std::size_t end) const {
    if (start >= data.size() || end >= data.size() || start > end)
        return PointsTable();

    PointsTable sliced_table;
    for (std::size_t i = start; i <= end; ++i)
        sliced_table.data.push_back(data[i]);

    return sliced_table;
}

PointsTable PointsTable::point_range(double x, std::size_t len) const {
    // ! Старая реализация
    // std::vector<std::size_t> indexes;

    // auto table_x = get_xs();
    // std::vector<double> distances;
    // distances.reserve(table_x.size());

    // for (const auto& x_value : table_x)
    //     distances.push_back(std::abs(x_value - x));

    // for (std::size_t i = 0; i < distances.size(); indexes.push_back(i++));

    // // Сортируем вектор индексов на основе расстояний
    // std::sort(indexes.begin(), indexes.end(), [&](std::size_t i, std::size_t j) {
    //     return (distances[i] != distances[j]) ? distances[i] < distances[j] : i < j;
    // });


    // std::size_t cnt = 0;
    // std::vector<std::size_t> nearest_indexes;
    // for (std::size_t i = 0; cnt <= len && i < indexes.size(); ++i) {
    //     nearest_indexes.push_back(indexes[i]);
    //     cnt++;
    // }
    
    // * Новый подход
    PointsTable result;

    std::size_t index;
    for (index = 0; index < data.size() && data[index].x < x; ++index);

    result.push_back(data[index ? --index : index]);
    ssize_t left = 0, right = 0;
    while (result.size() <= len && std::size_t(left) <= data.size() && std::size_t(right) <= data.size()) {
        if (index + (++right) < data.size())
            result.push_back(data[index + right]);
        if (result.size() <= len && ssize_t(index) - (++left) >= 0)
            result.push_back(data[index - left]);
    }
    
    result.sort();
    return result;
}

std::size_t PointsTable::size(void) const { return data.size(); }
errs_t PointsTable::is_incorrect(void) const { return err; }
void PointsTable::push_back(Point point) { data.push_back(point); }
Point PointsTable::operator[](std::size_t index) const { return data[index]; }

void PointsTable::sort() {
    std::sort(data.begin(), data.end(), [](const Point& fst, const Point& scd) {
        return fst.x < scd.x;
    });
}

std::ostream& operator<<(std::ostream& os, const PointsTable& data) {
    for (const auto &tmp : data.data)
        os << tmp << std::endl;
    return os;
}
