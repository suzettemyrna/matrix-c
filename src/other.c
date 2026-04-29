#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"
#include "helpers.h"

/* HELPERS FOR THIS MODULE */

static void fill_minor_matrix(int excluded_row, int excluded_column,
                              const matrix_t *A, matrix_t *minor) {
  for (int i = 0, minor_row = 0; i < A->rows; i++) {
    if (i != excluded_row) {
      for (int j = 0, minor_column = 0; j < A->columns; j++) {
        if (j != excluded_column) {
          minor->data[minor_row][minor_column] = A->data[i][j];
          minor_column++;
        }
      }
      minor_row++;
    }
  }
}

static int calc_cofactor(const matrix_t *A, int i, int j, double *result) {
  matrix_t minor;
  int res = matrix_create(A->rows - 1, A->columns - 1, &minor);
  if (res != MATRIX_OK) return res;

  fill_minor_matrix(i, j, A, &minor);

  double det = 0.0;
  res = matrix_determinant(&minor, &det);

  matrix_destroy(&minor);

  if (res != MATRIX_OK) return res;
  if (isnan(det) || isinf(det)) return MATRIX_ERR_CALC;

  int sign = ((i + j) % 2 == 0) ? 1 : -1;
  *result = sign * det;

  return MATRIX_OK;
}

// A_extend is the matrix expanded to the right for calculation by the
// Gauss-Jordan method
static void fill_extended_matrix(const matrix_t *A, matrix_t *A_extend) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A_extend->data[i][j] = A->data[i][j];
    }
    // Filling the matrix with zeros, and its diagonal with ones
    for (int j = 0; j < A->rows; j++) {
      A_extend->data[i][j + A->columns] = (i == j) ? 1 : 0;
    }
  }
}

static int gaussian_elimination(const matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    // If there is a 0 on the diagonal, then swap the rows
    if (fabs(A->data[i][i]) < EPS) {
      int flag = 0;
      for (int j = i + 1; j < A->rows && !flag; j++) {
        if (fabs(A->data[j][i]) > EPS) {
          for (int k = 0; k < A->columns; k++) {
            double swap = A->data[i][k];
            A->data[i][k] = A->data[j][k];
            A->data[j][k] = swap;
          }
          flag = 1;
        }
      }
    }
    // Reducing the diagonal element to 1
    double diag = A->data[i][i];
    if (fabs(diag) < EPS) return MATRIX_ERR_CALC;
    // Dividing its row by a diagonal element to make it a unit
    for (int l = 0; l < A->columns; l++) {
      A->data[i][l] /= diag;
    }
    // Reducing the elements in the column under the diagonal element to 0:
    // Subtracting the first row from the second and third rows multiplied by
    // the corresponding coefficient
    for (int m = 0; m < A->rows; m++) {
      if (m != i) {
        // Coefficient is an element from the column of the diagonal element and
        // from the corresponding row
        double factor = A->data[m][i];
        for (int n = 0; n < A->columns; n++) {
          double tmp = A->data[m][n] - factor * A->data[i][n];
          if (isnan(tmp) || isinf(tmp))
            return MATRIX_ERR_CALC;
          else
            A->data[m][n] -= factor * A->data[i][n];
        }
      }
    }
  }

  return MATRIX_OK;
}

static void extract_inverse(const matrix_t *extended, matrix_t *result) {
  int n = result->rows;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      result->data[i][j] = extended->data[i][j + n];
    }
  }
}

static int build_extended(const matrix_t *A, matrix_t *extended) {
  int res = matrix_create(A->rows, A->columns * 2, extended);
  if (res != MATRIX_OK) return res;

  fill_extended_matrix(A, extended);
  return MATRIX_OK;
}

/* PRIMARY FUNCS */

int matrix_transpose(const matrix_t *A, matrix_t *result) {
  if (!validate_one(A) || !result) return MATRIX_ERR_INVALID;

  int res = 0;
  if ((res = matrix_create(A->columns, A->rows, result)) != MATRIX_OK)
    return res;

  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->data[i][j] = A->data[j][i];
    }
  }

  return MATRIX_OK;
}

int matrix_determinant(const matrix_t *A, double *result) {
  if (!validate_one(A) || result == NULL) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_square(A)) return MATRIX_ERR_CALC;

  // If there is only one element in the matrix, then this element is the
  // determinant
  if (A->rows == 1) {
    *result = A->data[0][0];

  } else if (A->rows == 2) {
    *result = A->data[0][0] * A->data[1][1] - A->data[1][0] * A->data[0][1];

  } else {
    int det_sign = 1;
    *result = 0;
    for (int i = 0; i < A->columns; i++) {
      matrix_t minor;
      int res = matrix_create(A->rows - 1, A->columns - 1, &minor);
      if (res != MATRIX_OK) return res;

      fill_minor_matrix(0, i, A, &minor);

      double minor_det = 0;
      res = matrix_determinant(&minor, &minor_det);
      if (res != MATRIX_OK) {
        matrix_destroy(&minor);
        return res;
      }

      *result += det_sign * A->data[0][i] * minor_det;
      det_sign = -det_sign;

      matrix_destroy(&minor);
    }
    if (fabs(*result) < EPS) *result = 0.0;  // So that there is no -0
  }
  return MATRIX_OK;
}

int matrix_calc_complements(const matrix_t *A, matrix_t *result) {
  if (!validate_one(A) || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_square(A)) return MATRIX_ERR_CALC;

  int res = 0;

  if (A->rows == 1) {
    if ((res = matrix_create(1, 1, result)) != MATRIX_OK) return res;

    result->data[0][0] = 0;
    return MATRIX_OK;
  }

  if ((res = matrix_create(A->rows, A->columns, result)) != MATRIX_OK)
    return res;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      double value = 0.0;

      res = calc_cofactor(A, i, j, &value);
      if (res != MATRIX_OK) {
        matrix_destroy(result);
        return res;
      }

      result->data[i][j] = value;
    }
  }

  return MATRIX_OK;
}

int matrix_inverse(const matrix_t *A, matrix_t *result) {
  if (!validate_one(A) || !result) return MATRIX_ERR_INVALID;
  if (!is_matrix_ok(A) || !is_square(A)) return MATRIX_ERR_CALC;

  int res = 0;

  if (A->rows == 1) {
    if ((res = matrix_create(1, 1, result)) != MATRIX_OK) return res;

    if (fabs(A->data[0][0]) < EPS) return MATRIX_ERR_CALC;

    result->data[0][0] = 1.0 / A->data[0][0];
    return MATRIX_OK;
  }

  double det = 0.0;
  if ((res = matrix_determinant(A, &det)) != MATRIX_OK) return res;

  if (fabs(det) < EPS) return MATRIX_ERR_CALC;

  matrix_t extended;

  if ((res = build_extended(A, &extended)) != MATRIX_OK) return res;

  if ((res = gaussian_elimination(&extended)) != MATRIX_OK) {
    matrix_destroy(&extended);
    return res;
  }

  if ((res = matrix_create(A->rows, A->columns, result)) != MATRIX_OK) {
    matrix_destroy(&extended);
    return res;
  }

  extract_inverse(&extended, result);

  matrix_destroy(&extended);
  return MATRIX_OK;
}