#ifndef TESTS_H
#define TESTS_H

#include <check.h>

#include "../include/matrix.h"

void matrix_suit_execution(Suite *suite, int *failed_count, char *suite_name);

void initialize_matrix(matrix_t *A, double start_value, double iteration_step);

#endif