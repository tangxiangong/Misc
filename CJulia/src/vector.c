#include "vector.h"
#include <stdlib.h>
#include <assert.h>

int length(const Vector v) {
    return v.length;
}

Vector* vec_zeros(int n){
    Vector* p = (Vector*) malloc(sizeof(Vector));
    double* array = (double*) malloc(n*sizeof(double));
    for(int k=0; k<n;k++)
        *(array+k) = 0.0;
    p->data = array;
    p->length = n;
    return p;
}

Vector* vec_add(const Vector* lhs, const Vector* rhs) {
    assert(length(*rhs) == length(*rhs));
    int n = length(*rhs);
    Vector* result = vec_zeros(n);
    for(int i=0; i<n; ++i)
        result->data[i] = lhs->data[i] + rhs->data[i];
    return result;
}
