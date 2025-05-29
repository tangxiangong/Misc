#ifndef CJULIA_MATRIX_H
#define CJULIA_MATRIX_H
typedef struct {
    double(* data)[];
    int columns;  // 列
    int rows;  // 行
} Matrix;
Matrix* mat_zeros(int, int);
#endif //CJULIA_MATRIX_H
