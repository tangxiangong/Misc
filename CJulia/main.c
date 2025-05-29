#include <stdio.h>
//#include <julia.h>
//JULIA_DEFINE_FAST_TLS
#include "test.h"
#include "vector.h"
#include "matrix.h"

double func(double x)
{
    return x*x;
}

int main() {
    double a[] = {1, 2, 3, 4};
    double b[] = {-1,-2,-4,-3};
    Vector v1 = {a, 4};
    Vector v2 = {b, 4};
    Vector* v = vec_add(&v1, &v2);
    for(int i=0;i<4;i++)
        printf("%f ", v->data[i]);
    printf("\nHello, World!\n");
    return 0;
}
