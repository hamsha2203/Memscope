CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O0 -g

.PHONY: all run clean test
all: memscope_demo

memscope_demo: sample.c memscope.c memscope.h
	$(CC) $(CFLAGS) sample.c memscope.c -o memscope_demo

run: all
	python3 app.py

test: all
	python3 tests/test_memscope.py

clean:
	rm -f memscope_demo
