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

/** Tolerance for floating-point comparisons */
#define EPS 1e-7

/* ===========================
 * Types
 * =========================== */

/**
 * @brief Dynamically allocated matrix structure.
 *
 * Matrix elements are stored as a 2D array of doubles.
 * Memory is managed manually via matrix_create()
 * and matrix_destroy().
 */
typedef struct matrix_struct {
  double **data;
  int rows;
  int columns;
} matrix_t;

/**
 * @brief Status codes returned by matrix library functions.
 */
typedef enum {
  MATRIX_OK = 0,          /**< Operation completed successfully */
  MATRIX_ERR_INVALID = 1, /**< Invalid matrix or incorrect arguments */
  MATRIX_ERR_CALC = 2     /**< Calculation error */
} matrix_error_t;

/* ===========================
 * Memory management
 * =========================== */

/**
 * @brief Allocates memory for a matrix.
 *
 * Creates a matrix of size rows × columns
 * and initializes all elements to zero.
 *
 * Allocated memory must be released using matrix_destroy().
 *
 * @param rows Number of rows.
 * @param columns Number of columns.
 * @param result Pointer to resulting matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID on invalid arguments
 *         or allocation failure.
 */
int matrix_create(int rows, int columns, matrix_t *result);

/**
 * @brief Releases matrix memory.
 *
 * Frees all dynamically allocated memory associated
 * with the matrix and resets its fields.
 *
 * @param m Matrix to destroy.
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

/**
 * @brief Adds two matrices.
 *
 * Both matrices must have identical dimensions.
 *
 * @param A First matrix.
 * @param B Second matrix.
 * @param result Resulting matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC for incompatible matrix sizes
 *         or invalid numerical values.
 */
int matrix_add(const matrix_t *A, const matrix_t *B, matrix_t *result);

/**
 * @brief Subtracts one matrix from another.
 *
 * Both matrices must have identical dimensions.
 *
 * @param A Matrix to subtract from.
 * @param B Matrix to subtract.
 * @param result Resulting matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC for incompatible matrix sizes
 *         or invalid numerical values.
 */
int matrix_sub(const matrix_t *A, const matrix_t *B, matrix_t *result);

/**
 * @brief Multiplies matrix by a scalar value.
 *
 * @param A Source matrix.
 * @param number Scalar multiplier.
 * @param result Resulting matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC for invalid numerical values.
 */
int matrix_mul_scalar(const matrix_t *A, double number,
                      matrix_t *result);

/**
 * @brief Multiplies two matrices.
 *
 * Number of columns in the first matrix must equal
 * the number of rows in the second matrix.
 *
 * @param A Left matrix.
 * @param B Right matrix.
 * @param result Resulting matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC for incompatible matrix sizes
 *         or invalid numerical values.
 */
int matrix_mul(const matrix_t *A, const matrix_t *B,
               matrix_t *result);

/* ===========================
 * Advanced operations
 * =========================== */

/**
 * @brief Transposes a matrix.
 *
 * Rows become columns and columns become rows.
 *
 * @param A Source matrix.
 * @param result Transposed matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments.
 */
int matrix_transpose(const matrix_t *A, matrix_t *result);

/**
 * @brief Computes matrix of algebraic complements.
 *
 * The input matrix must be square.
 *
 * @param A Source matrix.
 * @param result Matrix of complements.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC if calculation cannot be performed.
 */
int matrix_calc_complements(const matrix_t *A,
                            matrix_t *result);

/**
 * @brief Calculates matrix determinant.
 *
 * The input matrix must be square.
 *
 * @param A Source matrix.
 * @param result Pointer to determinant value.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC if calculation cannot be performed.
 */
int matrix_determinant(const matrix_t *A, double *result);

/**
 * @brief Computes inverse matrix.
 *
 * The input matrix must be square and non-singular.
 *
 * @param A Source matrix.
 * @param result Inverse matrix.
 *
 * @return MATRIX_OK on success,
 *         MATRIX_ERR_INVALID for invalid arguments,
 *         MATRIX_ERR_CALC if inverse matrix does not exist
 *         or calculation fails.
 */
int matrix_inverse(const matrix_t *A, matrix_t *result);

#ifdef __cplusplus
}
#endif

#endif /* MATRIX_H */