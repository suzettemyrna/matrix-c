#include <math.h>
#include <stdlib.h>

#include "../include/matrix.h"
#include "helpers.h"

void zeroes(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A->data[i][j] = 0.0;
    }
  }
}

int is_matrix_ok(matrix_t *A) {
  int res = 1;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (isnan(A->data[i][j]) || isinf(A->data[i][j])) res = 0;
    }
  }
  return res;
}

int is_square(matrix_t *A) { return (A->rows == A->columns); }

matrix_t *create_minor(int excluded_row, int excluded_column, matrix_t *A) {
  if (A == NULL || A->data == NULL) {
    return NULL;
  }

  matrix_t *minor = calloc(1, sizeof(matrix_t));
  if (minor != NULL) {
    if (matrix_create(A->rows - 1, A->columns - 1, minor) == MATRIX_OK) {
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
  }

  return minor;
}

int copy_matrix(matrix_t *A, matrix_t *copy) {
  if (!copy) return MATRIX_ERR_INVALID;

  int res = MATRIX_OK;

  int copy_rows = A->rows, copy_columns = A->columns;
  if (matrix_create(copy_rows, copy_columns, copy) != MATRIX_OK) {
    res = MATRIX_ERR_INVALID;

  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        copy->data[i][j] = A->data[i][j];
      }
    }
  }

  return res;
}

// A_extend - расширенная вправо матрица для вычисления методом Гаусса-Жордана
void fill_extended_matrix(matrix_t *A, matrix_t *A_extend) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      A_extend->data[i][j] = A->data[i][j];
    }
    // Заполняем матрицу нулями, а её диагональ - единицами
    for (int j = 0; j < A->rows; j++) {
      A_extend->data[i][j + A->columns] = (i == j) ? 1 : 0;
    }
  }
}

int gaussian_elimination(matrix_t *A) {
  int res = MATRIX_OK;

  for (int i = 0; i < A->rows; i++) {
    // Если на диагонали стоит 0, то меняем строки местами
    if (fabs(A->data[i][i]) < EPS) {
      int flag = 0;
      for (int j = i + 1; j < A->rows && flag; j++) {
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
    // Приводим диагональный элемент к 1
    double diag = A->data[i][i];
    // Делим его строку на диагональный элемент, чтобы сделать его единицей
    for (int l = 0; l < A->columns; l++) {
      A->data[i][l] /= diag;
    }
    // Элементы в столбце под диагональным элементом приводим к 0:
    // Вычитаем из второй и третьей строк первую строку, умноженную на
    // соответствующий коэффициент
    for (int m = 0; m < A->rows; m++) {
      if (m != i) {
        // Коэффициент - элемент из столбца диагонального элемента и из
        // соответствущей строки
        double factor = A->data[m][i];
        for (int n = 0; n < A->columns; n++) {
          double tmp = A->data[m][n] - factor * A->data[i][n];
          if (isnan(tmp) || isinf(tmp))
            res = MATRIX_ERR_CALC;
          else
            A->data[m][n] -= factor * A->data[i][n];
        }
      }
    }
  }

  return res;
}

void initialize_matrix(matrix_t *A, double start_value,
                           double iteration_step) {
  if (A != NULL && A->data != NULL) {
    double value = start_value;

    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->data[i][j] = value;
        value += iteration_step;
      }
    }
  }
}