#ifndef MATHV_H
#define MATHV_H

typedef struct Matrix {
  float *data;
  int size, n, m;
} Matrix;


Matrix *matrixMult(Matrix *m1, Matrix *m2);
Matrix *matrixAdd(Matrix *m1, Matrix *m2);

#endif
