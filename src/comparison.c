#include <math.h>
#include <stdbool.h>

#include "../include/matrix.h"
#include "helpers.h"

int matrix_equal(const matrix_t *A, const matrix_t *B) {
  if (!A || !B || !A->data || !B->data) return false;

  int res = true;

  if (A->columns != B->columns || A->rows != B->rows) {
    res = false;

  } else {
    for (int i = 0; i < A->rows && res == true; i++) {
      for (int j = 0; j < A->columns && res == true; j++) {
        if (fabs(A->data[i][j] - B->data[i][j]) > EPS) res = false;
      }
    }
  }

  return res;
}