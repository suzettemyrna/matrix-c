#include "helpers.h"

#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"

int is_matrix_ok(const matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (isnan(A->data[i][j]) || isinf(A->data[i][j])) return 0;
    }
  }
  return 1;
}

int validate_one(const matrix_t *A) {
  if (!A || !A->data)
    return 0;
  else
    return 1;
}

int validate_two(const matrix_t *A, const matrix_t *B) {
  if (!A || !B || !A->data || !B->data)
    return 0;
  else
    return 1;
}

int is_square(const matrix_t *A) { return (A->rows == A->columns); }

int are_shapes_equal(const matrix_t *A, const matrix_t *B) {
  if (A->columns != B->columns || A->rows != B->rows)
    return 0;
  else
    return 1;
}