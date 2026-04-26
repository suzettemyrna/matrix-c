#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"
#include "helpers.h"

int matrix_transpose(matrix_t *A, matrix_t *result) {
  if (!A || !A->data || !result) return MATRIX_ERR_INVALID;

  int res = MATRIX_OK;

  if ((res = matrix_create(A->columns, A->rows, result)) == MATRIX_OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->data[i][j] = A->data[j][i];
      }
    }
  }

  return res;
}

int matrix_determinant(matrix_t *A, double *result) {
  if (!A || !A->data || result == NULL) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_square(A)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;
  // Если элемент в матрице один, то этот элемент и есть определитель
  if (A->rows == 1) {
    *result = A->data[0][0];

  } else if (A->rows == 2) {
    *result =
        A->data[0][0] * A->data[1][1] - A->data[1][0] * A->data[0][1];

  } else {
    int det_sign = 1;
    *result = 0;
    for (int i = 0; i < A->columns && res == MATRIX_OK; i++) {
      matrix_t *minor = create_minor(0, i, A);

      if (minor == NULL) {
        res = MATRIX_ERR_INVALID;
      } else {
        double minor_det = 0;
        res = matrix_determinant(minor, &minor_det);
        if (res == MATRIX_OK) {
          *result += det_sign * A->data[0][i] * minor_det;
          det_sign = -det_sign;
        }
        matrix_destroy(minor);
        free(minor);
        minor = NULL;
      }
    }
    if (fabs(*result) < EPS) *result = 0.0;  // Чтобы не было -0
  }

  return res;
}

int matrix_calc_complements(matrix_t *A, matrix_t *result) {
  if (!A || !A->data || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_square(A)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;

  if (A->rows == 1) {
    res = matrix_create(1, 1, result);
    if (res == MATRIX_OK) result->data[0][0] = 0;

  } else if ((res = matrix_create(A->rows, A->columns, result)) ==
             MATRIX_OK) {
    for (int i = 0; i < A->rows && res == MATRIX_OK; i++) {
      int sign = i % 2 == 0 ? 1 : -1;
      for (int j = 0; j < A->columns && res == MATRIX_OK; j++) {
        matrix_t *minor = create_minor(i, j, A);
        double det = 0.0;
        res = matrix_determinant(minor, &det);
        if (isnan(det) || isinf(det)) {
          res = MATRIX_ERR_CALC;
        } else {
          result->data[i][j] = sign * det;
          sign = -sign;
        }
        matrix_destroy(minor);
        free(minor);
        minor = NULL;
      }
    }
  }

  return res;
}

int matrix_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (!A || !A->data || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_square(A)) return MATRIX_ERR_CALC;

  int res = MATRIX_OK;
  matrix_t A_copy;
  // На случай матрицы 1х1:
  if (A->rows == 1) {
    if ((res = matrix_create(A->rows, A->columns, result)) == MATRIX_OK) {
      if (A->data[0][0] == 0)
        result->data[0][0] = 0;
      else
        result->data[0][0] = 1 / A->data[0][0];
    }

  } else {
    if ((res = copy_matrix(A, &A_copy)) == MATRIX_OK) {
      // Если детерминант равен нулю, невозможно создать обратную матрицу
      double det = 0.0;
      if ((res = matrix_determinant(&A_copy, &det)) == MATRIX_OK) {
        if (det == 0.0) res = MATRIX_ERR_CALC;
      }
    }
    matrix_destroy(&A_copy);

    //  Для расширенной матрицы
    matrix_t A_extend;
    if (matrix_create(A->rows, A->columns, result) != MATRIX_OK ||
        matrix_create(A->rows, A->columns * 2, &A_extend) != MATRIX_OK) {
      res = MATRIX_ERR_INVALID;

    } else {
      fill_extended_matrix(A, &A_extend);
      res = gaussian_elimination(&A_extend);
    }

    if (res == MATRIX_OK) {
      // Копируем правую часть расширенной матрицы в результат
      for (int i = 0; i < A_extend.rows; i++) {
        for (int j = 0; j < A_extend.rows; j++) {
          result->data[i][j] = A_extend.data[i][j + A_extend.rows];
        }
      }
    }
    matrix_destroy(&A_extend);
  }

  return res;
}