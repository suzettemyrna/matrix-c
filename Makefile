# ============================================
# Settings
# ============================================

# Name
LIB_NAME = libmatrix.a

# ============================================
# Paths
# ============================================

# Source directories
SRC_DIR = ./src
PUBLIC_HEADER_DIR = ./include
TEST_DIR = ./tests

# Build directories
BUILD_DIR = ./build
LIB_BUILD_DIR = $(BUILD_DIR)/lib

TEST_BUILD_DIR = $(TEST_DIR)/build
LOG_DIR = $(TEST_DIR)/logs
COV_DIR = $(TEST_DIR)/coverage

DOCS_DIR = ./docs
DOXY_DIR = $(DOCS_DIR)/doxygen

# ============================================
# Installation paths
# ============================================

PREFIX ?= /usr/local
BIN_DIR = $(PREFIX)/bin
INSTALL_LIB_DIR = $(PREFIX)/lib
INSTALL_INCLUDE_DIR = $(PREFIX)/include/libmatrix

# ============================================
# Compiler and flags
# ============================================

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = -lcheck -lm -lpthread
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LDFLAGS += -lrt -lsubunit
endif

# ============================================
# Source files
# ============================================

LIB_SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)

# ============================================
# Headers
# ============================================

PUBLIC_HEADER = $(PUBLIC_HEADER_DIR)/matrix.h
PRIVATE_HEADER = $(SRC_DIR)/include/matrix_utils.h
TESTS_HEADER = $(TEST_DIR)/tests.h

# ============================================
# Objects and targets
# ============================================

OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRCS))

STATIC_LIB = $(LIB_BUILD_DIR)/$(LIB_NAME)

TEST_BIN = $(TEST_BUILD_DIR)/test
TEST_OUTPUT = $(LOG_DIR)/test_output.log

# ============================================
# Main targets
# ============================================

.PHONY: all clean install uninstall dvi dist test tests clang-format clang-check format check

all: lib

lib: $(STATIC_LIB)

# Build static library (release)
$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_BUILD_DIR)
	ar rcs $@ $^
	@echo "=== Library build: $(STATIC_LIB) ==="

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================
# Install / Uninstall
# ============================================

install: $(STATIC_LIB)
	@mkdir -p $(INSTALL_LIB_DIR)
	@mkdir -p $(INSTALL_INCLUDE_DIR)

	cp $(STATIC_LIB) $(INSTALL_LIB_DIR)/
	cp $(PUBLIC_HEADER) $(INSTALL_INCLUDE_DIR)/

	@echo "=== Matrix library installed ==="

uninstall:
	rm -f $(INSTALL_LIB_DIR)/$(LIB_NAME)
	rm -f $(INSTALL_INCLUDE_DIR)/matrix.h

	@echo "=== Matrix library removed ==="

# ============================================
# Testing with coverage
# ============================================

test: tests

tests: $(STATIC_LIB)
	@mkdir -p $(TEST_BUILD_DIR)
	@mkdir -p $(LOG_DIR)
	@mkdir -p $(COV_DIR)

	$(CC) $(CFLAGS) --coverage \
		$(LIB_SRCS) $(TEST_SRC) \
		-o $(TEST_BIN) $(LDFLAGS)

	./$(TEST_BIN) > $(TEST_OUTPUT) 2>&1 || true

	@echo "=== Tests log: $(TEST_OUTPUT) ==="

	lcov -t "matrix_test" \
		-o $(COV_DIR)/coverage.info \
		-c -d .

	genhtml \
		-o $(COV_DIR)/report \
		$(COV_DIR)/coverage.info

	@rm -f tests/*.log
	@rm -f $(TEST_BUILD_DIR)/*.gcda $(TEST_BUILD_DIR)/*.gcno

	@if command -v xdg-open > /dev/null; then \
		xdg-open $(COV_DIR)/report/index.html; \
	elif command -v open > /dev/null; then \
		open $(COV_DIR)/report/index.html; \
	fi

# ============================================
# Doxygen documentation
# ============================================

dvi:
	@echo "=== Generating Doxygen documentation... ==="
	@if ! command -v doxygen >/dev/null 2>&1; then \
	  echo "doxygen not found (install: apt install doxygen / brew install doxygen)"; \
	  exit 1; \
	fi

	@mkdir -p $(DOXY_DIR)

	doxygen Doxyfile

	@echo "=== Doxygen documentation generated in $(DOXY_DIR)/html ==="

	@if command -v xdg-open >/dev/null 2>&1; then \
	  xdg-open $(DOXY_DIR)/html/index.html >/dev/null 2>&1 || true; \
	elif command -v open >/dev/null 2>&1; then \
	  open $(DOXY_DIR)/html/index.html >/dev/null 2>&1 || true; \
	else \
	  echo "Open manually: $(DOXY_DIR)/html/index.html"; \
	fi

# ============================================
# Distribution
# ============================================

dist: clean
	@mkdir -p dist
	tar -czf dist/matrix-1.0.tar.gz --exclude='dist' --exclude='.git' --exclude='*.o' --exclude='a.out' .
	@echo "=== Distribution package created: dist/matrix-1.0.tar.gz ==="

# ============================================
# Cleanup
# ============================================

clean:
	rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned"
	rm -rf $(TEST_BUILD_DIR) $(COV_DIR) $(TEST_DIR)/*.log
	@echo "Tests directory cleaned"
	rm -rf $(DOXY_DIR)
	@echo "Doxygen directory cleaned"
	rm -rf dist
	@echo "Distribution directory cleaned"
	@echo "=== Cleanup done ==="

# ============================================
# Code style (check and format)
# ============================================

check: clang-check

clang-check:
	@echo "=== Checking code style ==="
	@for file in $(LIB_SRCS) $(TEST_SRC) $(PUBLIC_HEADER) $(PRIVATE_HEADER) $(TESTS_HEADER); do \
			if [ -f "$$file" ]; then \
				clang-format -n -Werror "$$file" || exit 1; \
			fi; \
		done; \
		echo "=== Code style check passed ==="; \

format: clang-format

clang-format:
	@echo "=== Formatting code ==="
	@for file in $(LIB_SRCS) $(TEST_SRC) $(PUBLIC_HEADER) $(PRIVATE_HEADER) $(TESTS_HEADER); do \
			if [ -f "$$file" ]; then \
				clang-format -i -Werror "$$file" || exit 1; \
				echo "Formatted: $$file"; \
			fi; \
		done; \
		echo "=== Code formatting complete ==="; \