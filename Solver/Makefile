all:
	clang -Wall -Wextra -std=c99 -O3 -MMD *.c -c
	clang *.o -o solver

run:
	make
	./solver
	rm solver *.d

clean: 
	rm -rf *.[do] solver
	clear

	
