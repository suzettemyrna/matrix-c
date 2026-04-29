#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"
#include "helpers.h"

/* HELPERS FOR THIS MODULE */

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

static double add_op(double a, double b) { return a + b; }

static double sub_op(double a, double b) { return a - b; }

/* PRIMARY FUNCS */

int matrix_add(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  return matrix_elementwise(A, B, result, add_op);
}

int matrix_sub(const matrix_t *A, const matrix_t *B, matrix_t *result) {
  return matrix_elementwise(A, B, result, sub_op);
}

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