.PHONY: check clean

check:
	mkdir -p build
	cc -std=gnu11 -Wall -Wextra -Wno-unused-parameter -Iassignment -o build/page_table_demo assignment/os.c src/pt.c
	./build/page_table_demo

clean:
	rm -rf build
