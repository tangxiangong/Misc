#include <stdlib.h>
#include "matrix.h"

Matrix* mat_zeros(int columns, int rows) {
    Matrix* p = (Matrix*) malloc(sizeof(Matrix));
    double (*data)[columns] = malloc(columns*rows*sizeof(double));
    for(int i=0; i<rows; ++i) {
        for(int j=0; j<columns; ++j)
            *(data+i)[j] = 0.0;
    }
    p -> data = data;
    p -> columns = columns;
    p -> rows = rows;
    return p;
}
