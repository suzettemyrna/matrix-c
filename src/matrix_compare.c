#include <math.h>

#include "../include/matrix.h"
#include "include/matrix_utils.h"

/* ===========================
 * Public API
 * =========================== */

/**
 * @brief Compares two matrices with tolerance.
 *
 * Matrices are considered equal if all corresponding
 * elements differ by less than EPS.
 *
 * @param A First matrix
 * @param B Second matrix
 *
 * @return 1 if matrices are equal,
 *         0 otherwise
 */
int matrix_equal(const matrix_t *A, const matrix_t *B) {
  if (!validate_two(A, B)) return 0;
  if (!is_matrix_ok(A) || !is_matrix_ok(B)) return 0;

  if (!are_shapes_equal(A, B)) return 0;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->data[i][j] - B->data[i][j]) > EPS) return 0;
    }
  }

  return 1;
}