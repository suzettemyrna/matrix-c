# Matrix Library (C)

> A modular matrix computation library in C focused on memory management, numerical operations, and maintainable procedural design.

---

## Purpose

This project was built to demonstrate:

* dynamic memory management in C
* implementation of mathematical algorithms from scratch
* modular procedural architecture
* defensive programming and error handling
* unit testing of low-level code
* writing reusable library-style APIs

The project intentionally avoids external numerical libraries in order to focus on core systems programming skills and manual memory control.

---

## Overview

This is a lightweight matrix library written in pure C.

The library supports:

* dynamic matrix allocation
* arithmetic operations
* matrix transformations
* determinant calculation
* inverse matrix computation
* cofactor matrix generation

The project is structured as an independent reusable library rather than a single-purpose application.

Particular attention is given to:

* explicit ownership of dynamically allocated memory
* predictable API behavior
* validation of incorrect input
* separation between public API and internal helper logic

---

## Architecture

The project follows a simple modular structure.

### Public API

The `include/` directory exposes the library interface:

* matrix structure definition
* public enums and constants
* function declarations

Only the public API is intended to be used externally.

---

### Internal Modules

Implementation is separated into functional modules:

| Module                | Responsibility                    |
| --------------------- | --------------------------------- |
| `matrix_memory.c`     | memory allocation and destruction |
| `matrix_arithmetic.c` | arithmetic matrix operations      |
| `matrix_compare.c`    | matrix comparison                 |
| `matrix_advanced.c`   | advanced matrix algorithms        |
| `matrix_utils.c`      | internal helper utilities         |

Internal helper functions are isolated from the public API.

---

### Design Principles

* clear ownership of allocated memory
* explicit error propagation
* modular separation of responsibilities
* minimal hidden state
* predictable procedural API
* reusable library-oriented structure

---

## Implemented Operations

### Memory Management

* matrix creation
* matrix destruction
* matrix copying

---

### Arithmetic Operations

* matrix addition
* matrix subtraction
* scalar multiplication
* matrix multiplication

---

### Matrix Operations

* transpose
* determinant
* cofactor matrix
* inverse matrix

---

## Numerical Algorithms

The project includes manual implementations of several classical algorithms:

* recursive determinant calculation using minors
* cofactor matrix generation
* Gauss-Jordan elimination for inverse matrices

Floating-point comparisons are handled using epsilon tolerance.

---

## Error Handling

The library uses explicit status codes instead of hidden internal exceptions or aborts.

```c
typedef enum errors {
    MATRIX_OK = 0,
    MATRIX_ERR_INVALID = 1,
    MATRIX_ERR_CALC = 2
} matrix_status_t;
```

This approach makes behavior deterministic and easier to test.

---

## Project Structure

```text
matrix/
├── src/
│   ├── matrix_arithmetic.c
│   ├── matrix_compare.c
│   ├── matrix_utils.c
│   ├── matrix_memory.c
│   └── matrix_advanced.c
│
├── include/
│   ├── matrix.h
│   └── matrix_utils.h
│
├── tests/
│   ├── logs/
│   └── tests.c
│
├── coverage/
├── Makefile
└── README.md
```

---

## Build

### Requirements

* gcc
* make
* Check framework
* lcov (optional, for coverage reports)
* doxygen (optional, for documentation)

---

### Commands

```bash
make                # build static library
make test           # run unit tests + generate coverage report
make dvi            # generate Doxygen documentation
make clean          # remove build artifacts
```

---

## Tests

Testing is implemented with the **Check** framework.

The project includes:

* unit tests for all public functions
* validation of invalid input handling
* floating-point comparison tests
* edge-case testing
* coverage analysis with gcov/lcov

Test logs are automatically stored in:

```text
tests/logs/
```

Coverage reports are generated in:

```text
coverage/
```

---

## Usage Example

```c
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
```

---

## What this project demonstrates

This project demonstrates skills relevant to systems and backend development:

* dynamic memory management in C
* manual resource lifecycle control
* modular API-oriented design
* numerical algorithm implementation
* defensive programming
* low-level debugging
* unit testing of procedural code
* maintainable procedural architecture

Although relatively compact, the project is intentionally structured as a reusable low-level library rather than a monolithic educational exercise.

---

## Possible Improvements

* optimization of determinant calculation algorithms
* contiguous memory allocation for improved cache locality
* performance benchmarking for large matrices

---

## License

MIT License
