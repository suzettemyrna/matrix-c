#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"
#include "helpers.h"

/* ===========================
 * Internal helper functions
 * =========================== */

/**
 * @brief Applies an element-wise operation to two matrices.
 *
 * Creates a result matrix and applies the provided binary operation
 * to each pair of corresponding elements.
 *
 * Used internally for matrix addition and subtraction.
 *
 * @param A First matrix
 * @param B Second matrix
 * @param result Result matrix
 * @param op Binary operation function
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID on invalid input,
 *         MATRIX_ERR_CALC on calculation error
 */
static int matrix_elementwise(const matrix_t *A, const matrix_t *B,
                              matrix_t *result, double (*op)(double, double)) {
  if (!validate_two(A, B) || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_matrix_ok(B)) return MATRIX_ERR_CALC;

  if (!are_shapes_equal(A, B)) return MATRIX_ERR_CALC;

  int res = 0;
  if ((res = matrix_create(A->rows, A->columns, result)) != MATRIX_OK)
    return res;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      double tmp = op(A->data[i][j], B->data[i][j]);
      if (isnan(tmp) || isinf(tmp)) {
        matrix_destroy(result);
        return MATRIX_ERR_CALC;
      }
      result->data[i][j] = tmp;
    }
  }

  return MATRIX_OK;
}

/** @brief Addition operation helper. */
static double add_op(double a, double b) { return a + b; }

/** @brief Subtraction operation helper. */
static double sub_op(double a, double b) { return a - b; }

/* ===========================
 * Public API
 * =========================== */

/**
 * @brief Adds two matrices.
 *
 * Matrices must have identical dimensions.
 *
 * @param A First matrix
 * @param B Second matrix
 * @param result Result matrix
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID on invalid arguments,
 *         MATRIX_ERR_CALC on incompatible matrices
 */
int matrix_add(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  return matrix_elementwise(A, B, result, add_op);
}

/**
 * @brief Subtracts one matrix from another.
 */
int matrix_sub(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  return matrix_elementwise(A, B, result, sub_op);
}

/**
 * @brief Multiplies a matrix by a scalar value.
 *
 * @param A Source matrix
 * @param number Scalar multiplier
 * @param result Result matrix
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID on invalid arguments,
 *         MATRIX_ERR_CALC on calculation error
 */
int matrix_mul_scalar(const matrix_t *A, double number, matrix_t *result) {
  if (!validate_one(A) || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A)) return MATRIX_ERR_CALC;

  int res = 0;
  if ((res = matrix_create(A->rows, A->columns, result)) != MATRIX_OK)
    return res;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      double tmp = A->data[i][j] * number;
      if (isnan(tmp) || isinf(tmp)) {
        matrix_destroy(result);
        return MATRIX_ERR_CALC;
      } else {
        result->data[i][j] = tmp;
      }
    }
  }

  return MATRIX_OK;
}

/**
 * @brief Multiplies two matrices.
 *
 * Number of columns in the first matrix must equal
 * the number of rows in the second matrix.
 *
 * @param A Left matrix
 * @param B Right matrix
 * @param result Result matrix
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID on invalid arguments,
 *         MATRIX_ERR_CALC on incompatible matrices
 */
int matrix_mul(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  if (!validate_two(A, B) || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_matrix_ok(B)) return MATRIX_ERR_CALC;

  if (A->columns != B->rows) return MATRIX_ERR_CALC;

  int res = 0;
  if ((res = matrix_create(A->rows, B->columns, result)) != MATRIX_OK)
    return res;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      for (int k = 0; k < A->columns; k++) {
        result->data[i][j] += A->data[i][k] * B->data[k][j];
      }
    }
  }

  return res;
}