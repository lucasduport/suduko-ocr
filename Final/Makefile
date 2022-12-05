.SILENT: all
.SILENT: clean

all: ImageProcessing NeuralNetwork
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g `pkg-config --cflags sdl2` -c *.c
	clang -fsanitize=address -o main -lm -lSDL2 -lSDL2_image *.o

NeuralNetwork:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g -c ../NeuralNetwork/*.c
	rm -rf main*.o cellExtraction.o

ImageProcessing:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g `pkg-config --cflags sdl2` -c ../ImageProcessing/*.c
	rm -rf main*.o cellExtraction.o

run:
	make
	./main ../ImageProcessing/Images/image_01.jpeg

clean:
	rm -rf *.[do] main *.out
	rm -rf board_*/
	rm -rf COMMIT_MESSAGE
	clear
