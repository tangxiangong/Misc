#ifndef CJULIA_VECTOR_H
#define CJULIA_VECTOR_H
typedef struct {
    double* data;
    int length;
} Vector;
Vector* vec_zeros(int);
Vector* vec_add(const Vector*, const Vector*);
int length(Vector);

#endif //CJULIA_VECTOR_H
