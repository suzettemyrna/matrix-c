#include <math.h>

#include "../include/matrix.h"
#include "helpers.h"

int matrix_add(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !A->data || !B->data || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_matrix_ok(B)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;

  if (A->columns != B->columns || A->rows != B->rows) {
    res = MATRIX_ERR_CALC;

  } else if (matrix_create(A->rows, A->columns, result) != MATRIX_OK) {
    res = MATRIX_ERR_INVALID;

  } else {
    for (int i = 0; i < A->rows && res == MATRIX_OK; i++) {
      for (int j = 0; j < A->columns && res == MATRIX_OK; j++) {
        double tmp = A->data[i][j] + B->data[i][j];
        if (isnan(tmp) || isinf(tmp))
          res = MATRIX_ERR_CALC;
        else
          result->data[i][j] = tmp;
      }
    }
  }

  return res;
}

int matrix_sub(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !A->data || !B->data || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_matrix_ok(B)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;

  if (A->columns != B->columns || A->rows != B->rows) {
    res = MATRIX_ERR_CALC;

  } else if (matrix_create(A->rows, A->columns, result) != MATRIX_OK) {
    res = MATRIX_ERR_INVALID;

  } else {
    for (int i = 0; i < A->rows && res == MATRIX_OK; i++) {
      for (int j = 0; j < A->columns && res == MATRIX_OK; j++) {
        double tmp = A->data[i][j] - B->data[i][j];
        if (isnan(tmp) || isinf(tmp))
          res = MATRIX_ERR_CALC;
        else
          result->data[i][j] = tmp;
      }
    }
  }

  return res;
}

int matrix_mul_scalar(matrix_t *A, double number, matrix_t *result) {
  if (!A || !A->data || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;

  if (matrix_create(A->rows, A->columns, result) != MATRIX_OK) {
    res = MATRIX_ERR_INVALID;

  } else {
    for (int i = 0; i < A->rows && res == MATRIX_OK; i++) {
      for (int j = 0; j < A->columns && res == MATRIX_OK; j++) {
        double tmp = A->data[i][j] * number;
        if (isnan(tmp) || isinf(tmp))
          res = MATRIX_ERR_CALC;
        else
          result->data[i][j] = tmp;
      }
    }
  }

  return res;
}

int matrix_mul(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!A || !B || !A->data || !B->data || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_matrix_ok(B)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;

  if (A->columns != B->rows) {
    res = MATRIX_ERR_CALC;

  } else if (matrix_create(A->rows, B->columns, result) != MATRIX_OK) {
    res = MATRIX_ERR_INVALID;

  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->data[i][j] += A->data[i][k] * B->data[k][j];
        }
      }
    }
  }

  return res;
}