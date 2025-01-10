
all: run

build:
	zig cc ./src/*.c

run:build
	./a.out

