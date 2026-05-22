#ifndef HELPERS_H
#define HELPERS_H

#include "matrix.h"

/**
 * @brief Checks matrix elements for NaN and Inf values.
 */
int is_matrix_ok(const matrix_t *A);

/**
 * @brief Validates single matrix pointer.
 */
int validate_one(const matrix_t *A);

/**
 * @brief Validates two matrix pointers.
 */
int validate_two(const matrix_t *A, const matrix_t *B);

/**
 * @brief Checks whether matrix is square.
 */
int is_square(const matrix_t *A);

/**
 * @brief Checks whether two matrices have equal dimensions.
 */
int are_shapes_equal(const matrix_t *A, const matrix_t *B);

#endif /* HELPERS_H */