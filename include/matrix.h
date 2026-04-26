#ifndef MATRIX_H
#define MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file matrix.h
 * @brief Simple matrix manipulation library in C.
 *
 * Provides basic operations on dynamically allocated matrices,
 * including arithmetic operations and advanced computations.
 */

/* ===========================
 * Constants
 * =========================== */

 #define EPS 1e-7

/* ===========================
 * Types
 * =========================== */

/**
 * @brief Matrix structure
 *
 * Represents a 2D matrix of doubles.
 */
typedef struct matrix_struct {
  double **data;
  int rows;
  int columns;
} matrix_t;

/**
 * @brief Error codes returned by library functions
 */
typedef enum {
    MATRIX_OK = 0,
    MATRIX_ERR_INVALID = 1,
    MATRIX_ERR_CALC = 2
} matrix_error_t;

/* ===========================
 * Memory management
 * =========================== */

/**
 * @brief Creates a matrix with given dimensions
 *
 * @param rows Number of rows
 * @param cols Number of columns
 * @param result Pointer to matrix
 * @return MATRIX_OK on success, error code otherwise
 */
int matrix_create(int rows, int columns, matrix_t *result);

/**
 * @brief Frees matrix memory
 *
 * @param m Matrix to free
 */
void matrix_destroy(matrix_t *m);

/* ===========================
 * Comparison
 * =========================== */

/**
 * @brief Compares two matrices with tolerance
 *
 * @return 1 if equal, 0 otherwise
 */
int matrix_equal(const matrix_t *A, const matrix_t *B);

/* ===========================
 * Arithmetic operations
 * =========================== */

int matrix_add(matrix_t *a, matrix_t *b, matrix_t *result);
int matrix_sub(matrix_t *a, matrix_t *b, matrix_t *result);
int matrix_mul_scalar(matrix_t *A, double number, matrix_t *result);
int matrix_mul(matrix_t *A, matrix_t *B, matrix_t *result);

/* ===========================
 * Advanced operations
 * =========================== */

int matrix_transpose(matrix_t *A, matrix_t *result);
int matrix_calc_complements(matrix_t *A, matrix_t *result);
int matrix_determinant(matrix_t *A, double *result);
int matrix_inverse_matrix(matrix_t *A, matrix_t *result);

#ifdef __cplusplus
}
#endif

#endif /* MATRIX_H */