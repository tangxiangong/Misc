#include "test.h"
#include <math.h>

double trapezoid(double (*func)(double), double a, double b, double dx){
    double sum = 0;
    int n = (int) round((b-a)/dx);
    for (int i = 0; i < n; ++i) {
        sum += func(a + i*dx);
    }
    sum += (func(a)+func(b))/2;
    sum *= dx;
    return sum;
}