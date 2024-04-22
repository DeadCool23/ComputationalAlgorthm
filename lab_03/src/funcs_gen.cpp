#include "funcs_gen.hpp"

double test_func1(double x, double y, double z) {
#define EPS 1e-8 
    if (fabs(x + y) < EPS) return EPS - z;
    return 1 / (x + y) - z; 
#undef EPS
}
double test_func2(double x, double y, double z) { return x * y * z; }
double test_func3(double x, double y, double z) { return x - y + z; }
double test_func4(double x, double y, double z) { return exp(2 * x - y) * z; }

double square_func(double x, double y, double z) { return x * x + y * y + z * z; }