#include <stdlib.h>

#include "../include/matrix.h"
#include "helpers.h"

/* PRIMARY FUNCS */

int matrix_create(int rows, int columns, matrix_t *result) {
  if (!result || rows <= 0 || columns <= 0) return MATRIX_ERR_INVALID;

  result->rows = rows;
  result->columns = columns;

  result->data = calloc(rows, sizeof(double *));
  if (!result->data) return MATRIX_ERR_INVALID;

  for (int i = 0; i < rows; i++) {
    result->data[i] = calloc(columns, sizeof(double));
    if (!result->data[i]) {
      matrix_destroy(result);
      return MATRIX_ERR_INVALID;
    }
  }

  return MATRIX_OK;
}

void matrix_destroy(matrix_t *A) {
  if (A == NULL) return;

  if (A->data) {
    for (int i = 0; i < A->rows; i++) {
      if (A->data[i]) free(A->data[i]);
      A->data[i] = NULL;
    }
    free(A->data);
    A->data = NULL;
  }
  A->rows = 0;
  A->columns = 0;
}