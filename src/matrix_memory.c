#include <stdlib.h>

#include "../include/matrix.h"
#include "include/matrix_utils.h"

/* ===========================
 * Public API
 * =========================== */

/**
 * @brief Allocates memory for a matrix.
 *
 * Creates a matrix with the specified number of rows and columns.
 * Matrix elements are initialized to zero by calloc().
 *
 * @param rows Number of matrix rows
 * @param columns Number of matrix columns
 * @param result Pointer to destination matrix
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID on invalid arguments
 *         or allocation failure
 */
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

/**
 * @brief Frees all memory associated with a matrix.
 *
 * Releases memory allocated for matrix rows and resets
 * matrix fields to safe default values.
 *
 * @param A Matrix to destroy
 */
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