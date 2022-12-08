.SILENT: all
.SILENT: clean

all: Solver NeuralNetwork ImageProcessing UserInterface
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -c *.c
	clang `pkg-config --libs gtk+-3.0` -fsanitize=address -o main -lm -lSDL2 -lSDL2_image  *.o

Solver:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g -c ../Solver/*.c
	rm -rf main*.o

NeuralNetwork:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g -c ../NeuralNetwork/*.c
	rm -rf main*.o

ImageProcessing:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g `pkg-config --cflags sdl2` -c ../ImageProcessing/*.c
	rm -rf main*.o

UserInterface:
	clang -Wall -Wextra -std=c99 -O3 \
	-MMD -c -g `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2` -fsanitize=address ../UserInterface/*.c
	rm -rf main*.o

run:
	make
	./main ../ImageProcessing/Images/image_01.jpeg

clean:
	rm -rf *.[do] main *.out
	rm -rf board_*/
	rm -rf COMMIT_MESSAGE
	clear
