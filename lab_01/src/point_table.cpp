#include "point_table.hpp"

#include <cmath>
#include <algorithm>

std::size_t Point::diffs(void) const { return dy.size(); }

std::ostream& operator<<(std::ostream& os, const Point& data) {
    std::string diff = "";
    os << "x:" << data.x << " y:" << data.y;
    for (std::size_t i = 0; i < data.dy.size(); i++)
        os << " y" << (diff += "'") << ":" << data.dy[i];
    return os;
}

PointsTable::PointsTable(std::ifstream& file) {
    if (!file.is_open())
        err = ERR_FILE;

    std::string line;
    while (std::getline(file, line) && !err) {
        Point tmp_data;
        std::istringstream iss(line);
        try {
            if (!(iss >> tmp_data.x >> tmp_data.y))
                throw std::runtime_error("Incorrect line");
            
            double dy;
            while (iss >> dy)
                tmp_data.dy.push_back(dy);
            data.push_back(tmp_data);
        } catch (...) {
            err = ERR_IO;
        }
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

PointsTable PointsTable::reverse_func(void) {
#define EPS 1e-10
    PointsTable reversed_data;
    for (std::size_t i = 0; i < data.size(); i++) {
        if (data[i].diffs() && fabs(data[i].dy[0]) < EPS)
            reversed_data.err = ERR_CALC;

        Point rev_point = {
            .x = data[i].y,
            .y = data[i].x,
            .dy = data[i].dy
        };
        for (std::size_t j = 0; j < rev_point.diffs() && rev_point.dy[0]; j++) {
            switch (j) {
                case 0:
                    rev_point.dy[j] = 1 / rev_point.dy[j];
                    break;
                case 1:
                    rev_point.dy[j] = -rev_point.dy[j] / std::pow(rev_point.dy[j - 1], 3);
                    break;
                default:
                    reversed_data.err = ERR_CALC;
                    break;
            }
        }
        reversed_data.push_back(rev_point);
    }
    return reversed_data;
#undef EPS
}

PointsTable PointsTable::slice(std::size_t start, std::size_t end) const {
    if (start >= data.size() || end >= data.size() || start > end)
        return PointsTable();

    PointsTable sliced_table;
    for (std::size_t i = start; i <= end; ++i)
        sliced_table.data.push_back(data[i]);

    return sliced_table;
}

PointsTable PointsTable::point_range(double x, std::size_t len, bool consider_diff) const {
    std::vector<std::size_t> indexes;

    auto table_x = get_xs();
    std::vector<double> distances;
    distances.reserve(table_x.size());

    for (const auto& x_value : table_x)
        distances.push_back(std::abs(x_value - x));

    for (std::size_t i = 0; i < distances.size(); indexes.push_back(i++));

    // Сортируем вектор индексов на основе расстояний
    std::sort(indexes.begin(), indexes.end(), [&](std::size_t i, std::size_t j) {
        return (distances[i] != distances[j]) ? distances[i] < distances[j] : i < j;
    });


    std::size_t cnt = 0;
    std::vector<std::size_t> nearest_indexes;
    for (std::size_t i = 0; cnt <= len && i < indexes.size(); ++i) {
        nearest_indexes.push_back(indexes[i]);
        cnt += consider_diff ? 1 + data[indexes[i]].diffs() : 1;
    }
    
    PointsTable result;
    for (const auto& index : nearest_indexes)
        result.push_back(data[index]);
    result.sort();
    return result;
}




std::size_t PointsTable::size(void) const { return data.size(); }
errs_t PointsTable::is_incorrect(void) const { return err; }
void PointsTable::push_back(Point point) { data.push_back(point); }
Point PointsTable::operator[](std::size_t index) const { return data[index]; }

Point PointsTable::closest_to_zero_y(void) const {
    Point closest_point = data[0];
    for (std::size_t i = 1; i < data.size(); ++i)
        if (fabs(data[i].y) < fabs(closest_point.y))
            closest_point = data[i];

    return closest_point;
}

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
