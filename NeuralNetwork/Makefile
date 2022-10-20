.SILENT: all
.SILENT: run
.SILENT: clean

all:
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c Activations.c -o Activations.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c Cost.c -o Cost.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c IOHelper.c -o IOHelper.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c Layer.c -o Layer.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c main.c -o main.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c Network.c -o Network.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c Rand.c -o Rand.o
	clang -Weverything -g -fsanitize=address -ftrapv -fsanitize=undefined-trap -fsanitize-undefined-trap-on-error -O3 -Wall  -c Tools.c -o Tools.o
	clang -lm -o NeuralNetwork Activations.o Cost.o IOHelper.o Layer.o main.o Network.o Rand.o Tools.o -fsanitize=address

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
