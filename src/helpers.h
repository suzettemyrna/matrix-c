#ifndef HELPERS_H
#define HELPERS_H

#include "matrix.h"

void zeroes(matrix_t *A);
int is_matrix_ok(matrix_t *A);
int is_square(matrix_t *A);
matrix_t *create_minor(int excluded_row, int excluded_column, matrix_t *A);
int copy_matrix(matrix_t *A, matrix_t *copy);
void fill_extended_matrix(matrix_t *A, matrix_t *A_extend);
int gaussian_elimination(matrix_t *A);

#endif