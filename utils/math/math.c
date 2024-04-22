#include <stdlib.h>
#include <stdio.h>

#include "math.h"

Matrix *initMatrix(int n, int m) {
  Matrix *newMatrix;
  newMatrix = malloc(sizeof(Matrix));
  newMatrix->data = calloc(n * m, sizeof(float));
  newMatrix->n = n;
  newMatrix->m = m;
  return newMatrix;
}

float *getItem(Matrix *m1, int n, int m) {
  if (n >= m1->n || m >= m1->m) {
    return NULL;
  }
  return &m1->data[m1->m * n + m];
}

Matrix *matrixAdd(Matrix *m1, Matrix *m2) {
  Matrix *newMatrix = NULL;
  if (m1->m != m2->m || m1->n != m2->n) {
    return newMatrix;
  }
  newMatrix = initMatrix(m1->n, m1->m);
  for (int i = 0; i < m1->n; i++) {
    for (int j = 0; j < m1->m; j++) {
      *getItem(newMatrix, i, j) = *getItem(m1, i, j) + *getItem(m2, i, j);
    }
  }
  return newMatrix;
}

// Function to print a matrix
void printMatrix(Matrix *matrix) {
    for (int i = 0; i < matrix->n; i++) {
        for (int j = 0; j < matrix->m; j++) {
            printf("%.2f\t", *getItem(matrix, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void freeMatrix(Matrix *matrix) {
    free(matrix->data);
    free(matrix);
}

Matrix *matrixMult(Matrix *m1, Matrix *m2) {

}
