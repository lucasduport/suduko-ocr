.SILENT: all
.SILENT: run
.SILENT: clean

all:
	clang -v -std=c99 -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wextra -Wall -MMD -c *.c
	clang -lm -o NeuralNetwork *.o -fsanitize=address

run:
	make
	clear
	echo "========== Running main =========="
	./main
	echo "========== End of output =========="
	rm main *.d

clean:
	rm -rf *[.do] main
	clear
