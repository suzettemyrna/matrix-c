#ifndef HELPERS_H
#define HELPERS_H

#include "matrix.h"

int is_matrix_ok(const matrix_t *A);
int validate_one(const matrix_t *A);
int validate_two(const matrix_t *A, const matrix_t *B);
int is_square(const matrix_t *A);
int are_shapes_equal(const matrix_t *A, const matrix_t *B);

#endif