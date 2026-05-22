#include "include/matrix_utils.h"

#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"

/**
 * @brief Checks matrix elements for invalid floating-point values.
 *
 * Verifies that the matrix does not contain NaN or infinity values.
 *
 * @param A Matrix to validate
 *
 * @return 1 if matrix values are valid, 0 otherwise
 */
int is_matrix_ok(const matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (isnan(A->data[i][j]) || isinf(A->data[i][j])) return 0;
    }
  }
  return 1;
}

/**
 * @brief Validates a single matrix pointer.
 *
 * Checks matrix pointer and internal data pointer.
 *
 * @param A Matrix to validate
 *
 * @return 1 if valid, 0 otherwise
 */
int validate_one(const matrix_t *A) {
  if (!A || !A->data)
    return 0;
  else
    return 1;
}

/**
 * @brief Validates two matrix pointers.
 *
 * @param A First matrix
 * @param B Second matrix
 *
 * @return 1 if both matrices are valid, 0 otherwise
 */
int validate_two(const matrix_t *A, const matrix_t *B) {
  if (!A || !B || !A->data || !B->data)
    return 0;
  else
    return 1;
}

/**
 * @brief Checks whether a matrix is square.
 *
 * @param A Matrix to check
 *
 * @return 1 if square, 0 otherwise
 */
int is_square(const matrix_t *A) { return (A->rows == A->columns); }

/**
 * @brief Checks whether two matrices have equal dimensions.
 *
 * @param A First matrix
 * @param B Second matrix
 *
 * @return 1 if matrix shapes are equal, 0 otherwise
 */
int are_shapes_equal(const matrix_t *A, const matrix_t *B) {
  if (A->columns != B->columns || A->rows != B->rows)
    return 0;
  else
    return 1;
}