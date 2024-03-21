#include "splines.hpp"

// Кубическая Функция---
void CubeFunc::a_calc(double y) { a = y; };
void CubeFunc::b_calc(std::pair<double, double> ys, std::pair<double, double> cs, double h) {
    b = (ys.second - ys.first) / h - (h * (cs.second + 2 * cs.first) / 3);
}
void CubeFunc::c_calc(double next_c, double ksi, double teta) {
    c = ksi * next_c + teta; 
}
void CubeFunc::d_calc(std::pair<double, double> cs, double h) {
    d = (cs.second - cs.first) / (3 * h);
}

double CubeFunc::operator()(double x, double x0) const {
#define CUBE_FUCN_COEFS_CNT 4
    double res = 0;
    double dx = x - x0;
    double dx_accum = 1;
    for (std::size_t i = 0; i < CUBE_FUCN_COEFS_CNT; ++i) {
        double coef;
        if (i == 0) coef = a;
        else if (i == 1) coef = b;
        else if (i == 2) coef = c;
        else if (i == 3) coef = d;

        res += coef * dx_accum;
        dx_accum *= dx;
    }
    return res;
#undef CUBE_FUCN_COEFS_CNT
}

std::ostream& operator<<(std::ostream& os, const CubeFunc& func) {
    os << "a: " << func.a << ", b: " << func.b << ", c: " << func.c << ", d: " << func.d;
    return os;
}
//---

// Сплайн---
Splines::Splines(std::ifstream &file, std::pair<double, double> bords) : bords(bords) {
    PointsTable table = PointsTable(file);
    *this = Splines(table);
}
Splines::Splines(const PointsTable  &table, std::pair<double, double> bords) : bords(bords) {
    _x = table.get_xs();

    cnt = table.size() - 1;
    coefs_calc(table, bords);
}

double Splines::operator()(double x) const {
    std::size_t index = 0;
    for (; index < cnt - 1 && x > _x[index + 1]; ++index);

    return coefs[index](x, _x[index]);
}

void Splines::a_coefs_calc(std::vector<double> _y) {
    for (std::size_t i = 1; i <= cnt; ++i)
        coefs[i - 1].a_calc(_y[i - 1]);
}
void Splines::b_coefs_calc(const PointsTable &table, std::pair<double, double> bords) {
    auto is_end = [](std::size_t cnt, std::size_t index) {
        return index == cnt - 1;
    };

    auto iters = cnt; 
    for (std::size_t i = 0; i < iters; ++i) {
        double h;
        std::pair<double, double> cs, ys;

        if (!is_end(iters, i)) {
            h = (table[i + 1].x - table[i].x);
            ys = std::pair(table[i].y, table[i + 1].y);
            cs = std::pair(coefs[i].c, coefs[i + 1].c);
        } else {
            h = (table[table.size() - 1].x - table[table.size() - 2].x);
            ys = std::pair(table[table.size() - 2].y, table[table.size() - 1].y);
            cs = std::pair(coefs[coefs.size() - 1].c, bords.second / 2);
        }
        
        coefs[i].b_calc(ys, cs, h);
    }
}
void Splines::c_coefs_calc(const PointsTable &table, std::pair<double, double> bords) {
    auto ksis = std::vector<double>(table.size(), 0);
    auto tetas = std::vector<double>(table.size(), 0);

    auto ksi_calc = [](double prev_ksi, std::pair<double, double> hs) {
        return - hs.first / (2 * (hs.first + hs.second) + hs.second * prev_ksi);
    };
    auto teta_calc = [](std::pair<double, double> hs, 
                        std::pair<double, double> dys, 
                        double prev_ksi, double prev_teta) {
        return (3 * (dys.first / hs.first - dys.second / hs.second) - hs.second * prev_teta)
                /
               (hs.second * prev_ksi + 2 * (hs.first + hs.second));
    };

    coefs[0].c = bords.first / 2;
    ksis[1] = 0; tetas[1] = coefs[0].c;

    for (std::size_t i = 2; i < table.size(); ++i) {
        auto hs = std::pair(
            table[i].x - table[i - 1].x,
            table[i - 1].x - table[i - 2].x
        );

        ksis[i] = ksi_calc(ksis[i - 1], hs);
        auto dys = std::pair(
            table[i].y - table[i - 1].y,
            table[i - 1].y - table[i - 2].y
        );

        tetas[i] = teta_calc(hs, dys, ksis[i - 1], tetas[i - 1]);
    }
    coefs[coefs.size() - 1].c = tetas[tetas.size() - 1] 
                                + (bords.second / 2) * ksis[ksis.size() - 1];

    for (std::size_t i = cnt - 1; i > 0; --i)
        coefs[i - 1].c = tetas[i] + coefs[i].c * ksis[i]; 
}
void Splines::d_coefs_calc(void) {
    auto is_end = [](std::size_t cnt, std::size_t index) {
        return index == cnt - 1;
    };

    auto iters = cnt;
    for (std::size_t i = 0; i < iters; ++i) {
        double h;
        std::pair<double, double> cs;

        if (!is_end(iters, i)) {
            h = (_x[i + 1] - _x[i]);
            cs = std::pair(coefs[i].c, coefs[i + 1].c);
        } else {
            h = (_x[_x.size() - 1] - _x[_x.size() - 2]);
            cs = std::pair(coefs[coefs.size() - 1].c, bords.second / 2);
        }
        
        coefs[i].d_calc(cs, h);
    }
}

void Splines::coefs_calc(const PointsTable &table, std::pair<double, double> bords) {
    coefs.resize(cnt);
    a_coefs_calc(table.get_ys());
    c_coefs_calc(table, bords);
    b_coefs_calc(table, bords);
    d_coefs_calc();
}

std::ostream& operator<<(std::ostream& os, const Splines& splines) {
    for (std::size_t i = 0; i < splines.cnt; ++i) {
        os << "x: " << splines._x[i] << std::endl;
        os << "Coefs:\n" << splines.coefs[i] << std::endl << std::endl;
    }
    return os;
}
//---