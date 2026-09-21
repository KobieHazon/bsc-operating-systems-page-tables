CC ?= cc
CFLAGS ?= -std=gnu11 -Wall -Wextra -Wno-unused-parameter
CPPFLAGS += -Iassignment
ARGS ?=

.PHONY: all build run test check clean
all: build
build: build/page_table_demo

build/.directory:
	mkdir -p build
	touch $@

build/os.o: assignment/os.c assignment/os.h | build/.directory
	$(CC) $(CPPFLAGS) $(CFLAGS) -Dmain=course_smoke_main -c $< -o $@

build/page_table_demo: src/demo.c src/pt.c assignment/os.h build/os.o
	$(CC) $(CPPFLAGS) $(CFLAGS) src/demo.c src/pt.c build/os.o -o $@

build/test_page_tables: tests/test_page_tables.c src/pt.c assignment/os.h build/os.o
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_page_tables.c src/pt.c build/os.o -o $@

run: build
	./build/page_table_demo $(ARGS)

test: build/test_page_tables
	./build/test_page_tables

check: test

clean:
	rm -rf build
