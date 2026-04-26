# Matrix Library (C)

## Overview

A lightweight C library for working with matrices.

The library provides basic matrix operations, memory management, and more advanced mathematical computations such as determinant and inverse matrix.

This project focuses on clean structure, error handling, and modular design.

---

## Features

- Matrix creation and destruction
- Matrix comparison with tolerance
- Arithmetic operations:
  - Addition
  - Subtraction
  - Scalar multiplication
  - Matrix multiplication
- Advanced operations:
  - Transpose
  - Determinant
  - Cofactor matrix
  - Inverse matrix

---

## Project Structure


src/ implementation files
include/ public API (matrix.h)
tests/ unit tests
tests/logs/ test logs
coverage/ coverage reports


---

## Build

```bash
make
Run tests
make test

Test logs will be saved in:

tests/logs/
Code Coverage
make gcov_report

Coverage report will be generated in:

coverage/
Usage Example
#include "matrix.h"

int main() {
    matrix_t A, B, result;

    matrix_create(2, 2, &A);
    matrix_create(2, 2, &B);

    A.data[0][0] = 1;
    A.data[0][1] = 2;
    A.data[1][0] = 3;
    A.data[1][1] = 4;

    B.data[0][0] = 5;
    B.data[0][1] = 6;
    B.data[1][0] = 7;
    B.data[1][1] = 8;

    matrix_add(&A, &B, &result);

    matrix_destroy(&A);
    matrix_destroy(&B);
    matrix_destroy(&result);

    return 0;
}
Requirements
GCC (C11)
make
Check (for unit tests)
lcov (optional, for coverage)
Notes

This project was developed as part of learning C programming and focuses on:

modular design
memory management
error handling