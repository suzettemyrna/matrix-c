#include "tests.h"

#include <stdbool.h>
#include <stdlib.h>

/*  HELPER FUNC */

static void initialize_matrix(const matrix_t *A, double start_value,
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

/*  TEST OPTIONS */

START_TEST(matrix_create_1) {
  // true creation
  matrix_t A = {};
  ck_assert_int_eq(matrix_create(5, 5, &A), MATRIX_OK);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_create_2) {
  // 0 null pointer
  ck_assert_int_eq(matrix_create(5, 5, NULL), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_create_3) {
  // 0 incorrect rows/columns
  matrix_t A = {};
  ck_assert_int_eq(matrix_create(5, 0, &A), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_destroy_1) {
  // true remove
  matrix_t A = {};
  matrix_create(5, 5, &A);
  matrix_destroy(&A);
  ck_assert_ptr_null(A.data);
}
END_TEST

START_TEST(matrix_destroy_2) {
  // 0 handle null pointer
  matrix_destroy(NULL);
  ck_assert_int_eq(1, 1);
}
END_TEST

START_TEST(matrix_destroy_3) {
  // 0 handle MATRIX_ERR_INVALID structure
  matrix_t A = {};
  matrix_destroy(&A);
  ck_assert_int_eq(1, 1);
}
END_TEST

START_TEST(matrix_equal_1) {
  // true with initialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  initialize_matrix(&A, 21, 21);
  initialize_matrix(&B, 21, 21);
  ck_assert_int_eq(matrix_equal(&A, &B), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_equal_2) {
  // true with uninitialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  ck_assert_int_eq(matrix_equal(&A, &B), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_equal_3) {
  // true with a difference of less than 1e-7
  matrix_t A = {};
  matrix_t B = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  initialize_matrix(&A, 1, 0.000000001);
  initialize_matrix(&B, 1, 0.000000002);
  ck_assert_int_eq(matrix_equal(&A, &B), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_equal_4) {
  // 0 with different dimensions of matrices
  matrix_t A = {};
  matrix_t B = {};
  matrix_create(5, 5, &A);
  matrix_create(3, 4, &B);
  ck_assert_int_eq(matrix_equal(&A, &B), 0);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_equal_5) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  matrix_t B = {};
  matrix_create(5, 5, &A);
  ck_assert_int_eq(matrix_equal(&A, &B), 0);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_equal_6) {
  // 0 with different values
  matrix_t A = {};
  matrix_t B = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  initialize_matrix(&A, 21, 1);
  initialize_matrix(&B, 42, 1);
  ck_assert_int_eq(matrix_equal(&A, &B), 0);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_add_1) {
  // 0 error with wrong matrices
  matrix_t A = {};
  matrix_t B = {};
  ck_assert_int_eq(matrix_add(&A, &B, NULL), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_add_2) {
  // 0 with different dimensions of matrices
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_create(4, 4, &A);
  matrix_create(5, 5, &B);
  ck_assert_int_eq(matrix_add(&A, &B, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_add_3) {
  // 0 with non-finite value
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  initialize_matrix(&A, 1, 1);
  initialize_matrix(&B, 1, 1);
  B.data[3][3] = INFINITY;
  ck_assert_int_eq(matrix_add(&A, &B, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_add_4) {
  // sucess with initialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  matrix_create(5, 5, &eq_matrix);
  initialize_matrix(&A, 1, 1);
  initialize_matrix(&B, 1, 1);
  initialize_matrix(&eq_matrix, 2, 2);
  ck_assert_int_eq(matrix_add(&A, &B, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_add_5) {
  // sucess with uninitialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  matrix_create(5, 5, &eq_matrix);
  ck_assert_int_eq(matrix_add(&A, &B, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_add_6) {
  // sucess with task refence
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &B);
  matrix_create(3, 3, &eq_matrix);
  A.data[0][0] = 1, A.data[0][1] = 2, A.data[0][2] = 3;
  A.data[1][0] = 0, A.data[1][1] = 4, A.data[1][2] = 5;
  A.data[2][0] = 0, A.data[2][1] = 0, A.data[2][2] = 6;
  B.data[0][0] = 1, B.data[0][1] = 0, B.data[0][2] = 0;
  B.data[1][0] = 2, B.data[1][1] = 0, B.data[1][2] = 0;
  B.data[2][0] = 3, B.data[2][1] = 4, B.data[2][2] = 1;
  ck_assert_int_eq(matrix_add(&A, &B, &result), MATRIX_OK);
  eq_matrix.data[0][0] = 2, eq_matrix.data[0][1] = 2, eq_matrix.data[0][2] = 3;
  eq_matrix.data[1][0] = 2, eq_matrix.data[1][1] = 4, eq_matrix.data[1][2] = 5;
  eq_matrix.data[2][0] = 3, eq_matrix.data[2][1] = 4, eq_matrix.data[2][2] = 7;
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_sub_1) {
  // 0 error with wrong matrices
  matrix_t A = {};
  matrix_t B = {};
  ck_assert_int_eq(matrix_sub(&A, &B, NULL), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_sub_2) {
  // 0 with different dimensions of matrices
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_create(4, 4, &A);
  matrix_create(5, 5, &B);
  ck_assert_int_eq(matrix_sub(&A, &B, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_sub_3) {
  // 0 with non-finite value
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  initialize_matrix(&A, 1, 1);
  initialize_matrix(&B, 1, 1);
  B.data[3][3] = INFINITY;
  ck_assert_int_eq(matrix_sub(&A, &B, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_sub_4) {
  // sucess with initialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  matrix_create(5, 5, &eq_matrix);
  initialize_matrix(&A, 1, 1);
  initialize_matrix(&B, 1, 1);
  ck_assert_int_eq(matrix_sub(&A, &B, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_sub_5) {
  // sucess with uninitialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(5, 5, &A);
  matrix_create(5, 5, &B);
  matrix_create(5, 5, &eq_matrix);
  ck_assert_int_eq(matrix_sub(&A, &B, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_sub_6) {
  // sucess with task refence
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &B);
  matrix_create(3, 3, &eq_matrix);
  A.data[0][0] = 1, A.data[0][1] = 2, A.data[0][2] = 3;
  A.data[1][0] = 0, A.data[1][1] = 4, A.data[1][2] = 5;
  A.data[2][0] = 0, A.data[2][1] = 0, A.data[2][2] = 6;
  B.data[0][0] = 1, B.data[0][1] = 0, B.data[0][2] = 0;
  B.data[1][0] = 2, B.data[1][1] = 0, B.data[1][2] = 0;
  B.data[2][0] = 3, B.data[2][1] = 4, B.data[2][2] = 1;
  ck_assert_int_eq(matrix_sub(&A, &B, &result), MATRIX_OK);
  eq_matrix.data[0][0] = 0, eq_matrix.data[0][1] = 2, eq_matrix.data[0][2] = 3;
  eq_matrix.data[1][0] = -2, eq_matrix.data[1][1] = 4, eq_matrix.data[1][2] = 5;
  eq_matrix.data[2][0] = -3, eq_matrix.data[2][1] = -4,
  eq_matrix.data[2][2] = 5;
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_mul_scalar_1) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  double number = 3.14;
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_mul_scalar(&A, number, NULL), MATRIX_ERR_INVALID);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_mul_scalar_2) {
  // 0 with non-finite input double
  matrix_t A = {};
  matrix_t result = {};
  double number = INFINITY;
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_mul_scalar(&A, number, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_mul_scalar_3) {
  // 0 with with the resulting non-finite
  matrix_t A = {};
  matrix_t result = {};
  double number = 3;
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  A.data[2][2] = INFINITY;
  ck_assert_int_eq(matrix_mul_scalar(&A, number, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_mul_scalar_4) {
  // true with uninitialized values
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  double number = 3;
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &eq_matrix);
  ck_assert_int_eq(matrix_mul_scalar(&A, number, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_mul_scalar_5) {
  // true with initialized values
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  double number = 3;
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  matrix_create(3, 3, &eq_matrix);
  initialize_matrix(&eq_matrix, 3, 3);
  ck_assert_int_eq(matrix_mul_scalar(&A, number, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_mul_scalar_6) {
  // true with task reference values
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  double number = 2;
  matrix_create(3, 3, &A);
  A.data[0][0] = 1, A.data[0][1] = 2, A.data[0][2] = 3;
  A.data[1][0] = 0, A.data[1][1] = 4, A.data[1][2] = 2;
  A.data[2][0] = 2, A.data[2][1] = 3, A.data[2][2] = 4;
  matrix_create(3, 3, &eq_matrix);
  eq_matrix.data[0][0] = 2, eq_matrix.data[0][1] = 4, eq_matrix.data[0][2] = 6;
  eq_matrix.data[1][0] = 0, eq_matrix.data[1][1] = 8, eq_matrix.data[1][2] = 4;
  eq_matrix.data[2][0] = 4, eq_matrix.data[2][1] = 6, eq_matrix.data[2][2] = 8;
  ck_assert_int_eq(matrix_mul_scalar(&A, number, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_mul_1) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_mul(&A, NULL, NULL), MATRIX_ERR_INVALID);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_mul_2) {
  // 0 with with the resulting non-finite
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &B);
  initialize_matrix(&A, 1, 1);
  B.data[2][2] = INFINITY;
  ck_assert_int_eq(matrix_mul(&A, &B, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_mul_3) {
  // 0 with different dimensions of matrices
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_create(3, 2, &A);
  matrix_create(3, 2, &B);
  ck_assert_int_eq(matrix_mul(&A, &B, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
  matrix_destroy(&B);
}
END_TEST

START_TEST(matrix_mul_4) {
  // true with uninitialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &B);
  matrix_create(3, 3, &eq_matrix);
  ck_assert_int_eq(matrix_mul(&A, &B, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_mul_5) {
  // true with initialized values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  matrix_create(3, 3, &B);
  initialize_matrix(&B, 1, 1);
  matrix_create(3, 3, &eq_matrix);
  eq_matrix.data[0][0] = 30, eq_matrix.data[0][1] = 36,
  eq_matrix.data[0][2] = 42;
  eq_matrix.data[1][0] = 66, eq_matrix.data[1][1] = 81,
  eq_matrix.data[1][2] = 96;
  eq_matrix.data[2][0] = 102, eq_matrix.data[2][1] = 126,
  eq_matrix.data[2][2] = 150;
  ck_assert_int_eq(matrix_mul(&A, &B, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_mul_6) {
  // true with task reference values
  matrix_t A = {};
  matrix_t B = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 2, &A);
  A.data[0][0] = 1, A.data[0][1] = 4;
  A.data[1][0] = 2, A.data[1][1] = 5;
  A.data[2][0] = 3, A.data[2][1] = 6;
  matrix_create(2, 3, &B);
  B.data[0][0] = 1, B.data[0][1] = -1, B.data[0][2] = 1;
  B.data[1][0] = 2, B.data[1][1] = 3, B.data[1][2] = 4;
  matrix_create(3, 3, &eq_matrix);
  eq_matrix.data[0][0] = 9, eq_matrix.data[0][1] = 11,
  eq_matrix.data[0][2] = 17;
  eq_matrix.data[1][0] = 12, eq_matrix.data[1][1] = 13,
  eq_matrix.data[1][2] = 22;
  eq_matrix.data[2][0] = 15, eq_matrix.data[2][1] = 15,
  eq_matrix.data[2][2] = 27;
  ck_assert_int_eq(matrix_mul(&A, &B, &result), MATRIX_OK);
  // matrix_print_matrix(&result);
  // matrix_print_matrix(&eq_matrix);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_transpose_1) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  matrix_t result = {};
  ck_assert_int_eq(matrix_transpose(&A, &result), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_transpose_2) {
  // true with unininitialized matrix 3x2
  matrix_t A = {};
  matrix_t result = {};
  matrix_create(3, 2, &A);
  ck_assert_int_eq(matrix_transpose(&A, &result), MATRIX_OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 3);
  matrix_destroy(&A);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_transpose_3) {
  // true with unininitialized matrix 2x3
  matrix_t A = {};
  matrix_t result = {};
  matrix_create(2, 3, &A);
  ck_assert_int_eq(matrix_transpose(&A, &result), MATRIX_OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  matrix_destroy(&A);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_transpose_4) {
  // true with initialized matrix 2x3
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(2, 3, &A);
  matrix_create(3, 2, &eq_matrix);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_transpose(&A, &result), MATRIX_OK);
  eq_matrix.data[0][0] = 1, eq_matrix.data[0][1] = 4;
  eq_matrix.data[1][0] = 2, eq_matrix.data[1][1] = 5;
  eq_matrix.data[2][0] = 3, eq_matrix.data[2][1] = 6;
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 2);
  matrix_destroy(&A);
  matrix_destroy(&eq_matrix);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_transpose_5) {
  // true with task reference values and 3x2 matrix
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 2, &A);
  matrix_create(2, 3, &eq_matrix);
  A.data[0][0] = 1, A.data[0][1] = 4;
  A.data[1][0] = 2, A.data[1][1] = 5;
  A.data[2][0] = 3, A.data[2][1] = 6;
  ck_assert_int_eq(matrix_transpose(&A, &result), MATRIX_OK);
  eq_matrix.data[0][0] = 1, eq_matrix.data[0][1] = 2, eq_matrix.data[0][2] = 3;
  eq_matrix.data[1][0] = 4, eq_matrix.data[1][1] = 5, eq_matrix.data[1][2] = 6;
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 3);
  matrix_destroy(&A);
  matrix_destroy(&eq_matrix);
  matrix_destroy(&result);
}
END_TEST

START_TEST(matrix_calc_complements_1) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  matrix_create(3, 3, &A);
  ck_assert_int_eq(matrix_calc_complements(&A, NULL), MATRIX_ERR_INVALID);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_calc_complements_2) {
  // 0 with vector matrix (rows or cols == 1)
  matrix_t A = {};
  matrix_t result = {};
  matrix_create(1, 3, &A);
  initialize_matrix(&A, 1, 3);
  ck_assert_int_eq(matrix_calc_complements(&A, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_calc_complements_3) {
  // true with task reference values
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &eq_matrix);
  A.data[0][0] = 1, A.data[0][1] = 2, A.data[0][2] = 3;
  A.data[1][0] = 0, A.data[1][1] = 4, A.data[1][2] = 2;
  A.data[2][0] = 5, A.data[2][1] = 2, A.data[2][2] = 1;
  ck_assert_int_eq(matrix_calc_complements(&A, &result), MATRIX_OK);
  eq_matrix.data[0][0] = 0, eq_matrix.data[0][1] = 10,
  eq_matrix.data[0][2] = -20;
  eq_matrix.data[1][0] = 4, eq_matrix.data[1][1] = -14,
  eq_matrix.data[1][2] = 8;
  eq_matrix.data[2][0] = -8, eq_matrix.data[2][1] = -2,
  eq_matrix.data[2][2] = 4;
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_determinant_1) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  double det = 0;
  ck_assert_int_eq(matrix_determinant(&A, &det), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_determinant_2) {
  // 0 with non-square matrix
  matrix_t A = {};
  double det = 0;
  matrix_create(3, 2, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_determinant(&A, &det), MATRIX_ERR_CALC);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_determinant_3) {
  // true with task reference values
  matrix_t A = {};
  double det = 0;
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_determinant(&A, &det), MATRIX_OK);
  ck_assert_double_eq(det, 0);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_determinant_4) {
  // true with 2x2 matrix
  matrix_t A = {};
  double det = 0;
  matrix_create(2, 2, &A);
  initialize_matrix(&A, 3, 3);
  ck_assert_int_eq(matrix_determinant(&A, &det), MATRIX_OK);
  ck_assert_double_eq(det, -18);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_determinant_5) {
  // true with 1x1 matrix
  matrix_t A = {};
  double det = 0;
  matrix_create(1, 1, &A);
  A.data[0][0] = 21;
  ck_assert_int_eq(matrix_determinant(&A, &det), MATRIX_OK);
  ck_assert_double_eq(det, 21);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_inverse_1) {
  // 0 with MATRIX_ERR_INVALID
  matrix_t A = {};
  matrix_t result = {};
  ck_assert_int_eq(matrix_inverse(&A, &result), MATRIX_ERR_INVALID);
}
END_TEST

START_TEST(matrix_inverse_2) {
  // 0 when matrix has determinant equal to zero
  matrix_t A = {};
  matrix_t result = {};
  matrix_create(3, 3, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_inverse(&A, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_inverse_3) {
  // 0 with non-square matrix
  matrix_t A = {};
  matrix_t result = {};
  matrix_create(5, 3, &A);
  initialize_matrix(&A, 1, 1);
  ck_assert_int_eq(matrix_inverse(&A, &result), MATRIX_ERR_CALC);
  matrix_destroy(&A);
}
END_TEST

START_TEST(matrix_inverse_4) {
  // true matrix 1x1
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(1, 1, &A);
  matrix_create(1, 1, &eq_matrix);
  A.data[0][0] = 21;
  eq_matrix.data[0][0] = 1.0 / 21.0;
  ck_assert_int_eq(matrix_inverse(&A, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_inverse_5) {
  // true with task reference values
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};
  matrix_create(3, 3, &A);
  matrix_create(3, 3, &eq_matrix);
  A.data[0][0] = 2, A.data[0][1] = 5, A.data[0][2] = 7;
  A.data[1][0] = 6, A.data[1][1] = 3, A.data[1][2] = 4;
  A.data[2][0] = 5, A.data[2][1] = -2, A.data[2][2] = -3;
  eq_matrix.data[0][0] = 1, eq_matrix.data[0][1] = -1, eq_matrix.data[0][2] = 1;
  eq_matrix.data[1][0] = -38, eq_matrix.data[1][1] = 41,
  eq_matrix.data[1][2] = -34;
  eq_matrix.data[2][0] = 27, eq_matrix.data[2][1] = -29,
  eq_matrix.data[2][2] = 24;
  ck_assert_int_eq(matrix_inverse(&A, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);
  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

START_TEST(matrix_inverse_6) {
  // true with swap case
  matrix_t A = {};
  matrix_t result = {};
  matrix_t eq_matrix = {};

  matrix_create(2, 2, &A);
  matrix_create(2, 2, &eq_matrix);

  // forcing row swap in gaussian elimination
  A.data[0][0] = 0;
  A.data[0][1] = 1;
  A.data[1][0] = 2;
  A.data[1][1] = 3;

  // expected inverse
  // det = (0*3 - 2*1) = -2
  // inverse =
  // [ -3/2   1/2 ]
  // [  1     0   ]

  eq_matrix.data[0][0] = -1.5;
  eq_matrix.data[0][1] = 0.5;
  eq_matrix.data[1][0] = 1.0;
  eq_matrix.data[1][1] = 0.0;

  ck_assert_int_eq(matrix_inverse(&A, &result), MATRIX_OK);
  ck_assert_int_eq(matrix_equal(&result, &eq_matrix), 1);

  matrix_destroy(&A);
  matrix_destroy(&result);
  matrix_destroy(&eq_matrix);
}
END_TEST

/*  TEST SUITES */

Suite *matrix_create_suite(void) {
  Suite *suite = suite_create("matrix_create");
  TCase *tc_core = tcase_create("core_of_create_matrix");
  tcase_add_test(tc_core, matrix_create_1);
  tcase_add_test(tc_core, matrix_create_2);
  tcase_add_test(tc_core, matrix_create_3);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_destroy_suite(void) {
  Suite *suite = suite_create("matrix_destroy");
  TCase *tc_core = tcase_create("core_of_remove_matrix");
  tcase_add_test(tc_core, matrix_destroy_1);
  tcase_add_test(tc_core, matrix_destroy_2);
  tcase_add_test(tc_core, matrix_destroy_3);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_equal_suite(void) {
  Suite *suite = suite_create("matrix_equal");
  TCase *tc_core = tcase_create("core_of_eq_matrix");
  tcase_add_test(tc_core, matrix_equal_1);
  tcase_add_test(tc_core, matrix_equal_2);
  tcase_add_test(tc_core, matrix_equal_3);
  tcase_add_test(tc_core, matrix_equal_4);
  tcase_add_test(tc_core, matrix_equal_5);
  tcase_add_test(tc_core, matrix_equal_6);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_add_suite(void) {
  Suite *suite = suite_create("matrix_add");
  TCase *tc_core = tcase_create("core_of_sum_matrix");
  tcase_add_test(tc_core, matrix_add_1);
  tcase_add_test(tc_core, matrix_add_2);
  tcase_add_test(tc_core, matrix_add_3);
  tcase_add_test(tc_core, matrix_add_4);
  tcase_add_test(tc_core, matrix_add_5);
  tcase_add_test(tc_core, matrix_add_6);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_sub_suite(void) {
  Suite *suite = suite_create("matrix_sub");
  TCase *tc_core = tcase_create("core_of_sub_matrix");
  tcase_add_test(tc_core, matrix_sub_1);
  tcase_add_test(tc_core, matrix_sub_2);
  tcase_add_test(tc_core, matrix_sub_3);
  tcase_add_test(tc_core, matrix_sub_4);
  tcase_add_test(tc_core, matrix_sub_5);
  tcase_add_test(tc_core, matrix_sub_6);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_mul_scalar_suite(void) {
  Suite *suite = suite_create("matrix_mul_scalar");
  TCase *tc_core = tcase_create("core_of_mult_number");
  tcase_add_test(tc_core, matrix_mul_scalar_1);
  tcase_add_test(tc_core, matrix_mul_scalar_2);
  tcase_add_test(tc_core, matrix_mul_scalar_3);
  tcase_add_test(tc_core, matrix_mul_scalar_4);
  tcase_add_test(tc_core, matrix_mul_scalar_5);
  tcase_add_test(tc_core, matrix_mul_scalar_6);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_mul_suite(void) {
  Suite *suite = suite_create("matrix_mul");
  TCase *tc_core = tcase_create("core_of_mult_matrix");
  tcase_add_test(tc_core, matrix_mul_1);
  tcase_add_test(tc_core, matrix_mul_2);
  tcase_add_test(tc_core, matrix_mul_3);
  tcase_add_test(tc_core, matrix_mul_4);
  tcase_add_test(tc_core, matrix_mul_5);
  tcase_add_test(tc_core, matrix_mul_6);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_transpose_suite(void) {
  Suite *suite = suite_create("matrix_transpose");
  TCase *tc_core = tcase_create("core_of_transpose");
  tcase_add_test(tc_core, matrix_transpose_1);
  tcase_add_test(tc_core, matrix_transpose_2);
  tcase_add_test(tc_core, matrix_transpose_3);
  tcase_add_test(tc_core, matrix_transpose_4);
  tcase_add_test(tc_core, matrix_transpose_5);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_calc_complements_suite(void) {
  Suite *suite = suite_create("matrix_calc_complements");
  TCase *tc_core = tcase_create("core_of_calc_complements");
  tcase_add_test(tc_core, matrix_calc_complements_1);
  tcase_add_test(tc_core, matrix_calc_complements_2);
  tcase_add_test(tc_core, matrix_calc_complements_3);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_determinant_suite(void) {
  Suite *suite = suite_create("matrix_determinant");
  TCase *tc_core = tcase_create("core_of_determinant");
  tcase_add_test(tc_core, matrix_determinant_1);
  tcase_add_test(tc_core, matrix_determinant_2);
  tcase_add_test(tc_core, matrix_determinant_3);
  tcase_add_test(tc_core, matrix_determinant_4);
  tcase_add_test(tc_core, matrix_determinant_5);
  suite_add_tcase(suite, tc_core);

  return suite;
}

Suite *matrix_inverse_suite(void) {
  Suite *suite = suite_create("matrix_inverse");
  TCase *tc_core = tcase_create("core_of_inverse");
  tcase_add_test(tc_core, matrix_inverse_1);
  tcase_add_test(tc_core, matrix_inverse_2);
  tcase_add_test(tc_core, matrix_inverse_3);
  tcase_add_test(tc_core, matrix_inverse_4);
  tcase_add_test(tc_core, matrix_inverse_5);
  tcase_add_test(tc_core, matrix_inverse_6);
  suite_add_tcase(suite, tc_core);

  return suite;
}

/*  SUITES EXECUTION SCRIPT */

void matrix_suit_execution(Suite *suite, int *failed_count, char *suite_name) {
  SRunner *suite_runner = srunner_create(suite);
  srunner_set_log(suite_runner, suite_name);
  srunner_run_all(suite_runner, CK_NORMAL);
  *failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
}

/*  TESTS MAIN */

int main(void) {
  int failed_count = 0;

  matrix_suit_execution(matrix_create_suite(), &failed_count,
                        "tests/create_matrix.log");
  matrix_suit_execution(matrix_destroy_suite(), &failed_count,
                        "tests/remove_matrix.log");
  matrix_suit_execution(matrix_equal_suite(), &failed_count,
                        "tests/eq_matrix.log");
  matrix_suit_execution(matrix_add_suite(), &failed_count,
                        "tests/sum_matrix.log");
  matrix_suit_execution(matrix_sub_suite(), &failed_count,
                        "tests/sub_matrix.log");
  matrix_suit_execution(matrix_mul_scalar_suite(), &failed_count,
                        "tests/mult_number.log");
  matrix_suit_execution(matrix_mul_suite(), &failed_count,
                        "tests/mult_matrix.log");
  matrix_suit_execution(matrix_transpose_suite(), &failed_count,
                        "tests/transpose.log");
  matrix_suit_execution(matrix_calc_complements_suite(), &failed_count,
                        "tests/calc_complements.log");
  matrix_suit_execution(matrix_determinant_suite(), &failed_count,
                        "tests/determinant.log");
  matrix_suit_execution(matrix_inverse_suite(), &failed_count,
                        "tests/inverse_matrix.log");

  return failed_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}