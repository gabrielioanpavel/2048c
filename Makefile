all: 2048
2048: 2048.c
	gcc -Wall 2048.c -o exec -lncurses

.PHONY: run clean build
build: 2048
run: 2048
	./exec
clean: 2048
	rm -f exec