CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
LDFLAGS = -lcheck -lm -lpthread
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LDFLAGS += -lrt -lsubunit
endif

SRC_DIR = src
TEST_DIR = tests
LOG_DIR = $(TEST_DIR)/logs
COV_DIR = coverage

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

TEST_SRC = $(wildcard $(TEST_DIR)/*.c)
TEST_OUTPUT = $(LOG_DIR)/test_output.log

TARGET = libmatrix.a
TEST_BIN = test

.PHONY: all install clean test gcov_report style format

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^
	@echo "=================================="
	@echo "Library is ready: $(TARGET)"
	@echo "=================================="

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install:
	cp $(TARGET) /usr/local/lib/
	cp include/matrix.h /usr/local/include/

test: $(TARGET)
	mkdir -p $(LOG_DIR)
	$(CC) $(CFLAGS) $(TEST_SRC) $(TARGET) -o $(TEST_BIN) $(LDFLAGS)
	./$(TEST_BIN) > $(TEST_OUTPUT) 2>&1 || true
	@echo "=================================="
	@echo "Tests log is here: $(TEST_OUTPUT)"
	@echo "=================================="

gcov_report: clean
	mkdir -p $(COV_DIR)
	$(CC) $(CFLAGS) --coverage $(SRCS) $(TEST_SRC) -o $(TEST_BIN) $(LDFLAGS)
	./$(TEST_BIN)
	lcov -t "test" -o $(COV_DIR)/coverage.info -c -d .
	genhtml -o $(COV_DIR)/report $(COV_DIR)/coverage.info
	xdg-open $(COV_DIR)/report/index.html || open $(COV_DIR)/report/index.html

style:
	clang-format -n $(SRC_DIR)/*.c $(SRC_DIR)/*.h include/*.h $(TEST_DIR)/*.c $(TEST_DIR)/*.h

format:
	clang-format -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h include/*.h $(TEST_DIR)/*.c $(TEST_DIR)/*.h

clean:
	rm -rf $(OBJS) $(TARGET) $(TEST_BIN) *.gcda *.gcno *.info report