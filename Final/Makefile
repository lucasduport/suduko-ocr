.SILENT: all
.SILENT: clean

all:
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g `pkg-config --cflags sdl2` -c ../ImageProcessing/*.c
	rm -rf main.o
	clang -fsanitize=address -fsanitize=undefined -Wall -Wextra -std=c99 -O3 \
	-MMD -g `pkg-config --cflags sdl2` -c *.c
	clang -fsanitize=address -o main -lm -lSDL2 -lSDL2_image *.o

clean:
	rm -rf *.[do] main *.out
	rm -rf board_*/
	rm -rf COMMIT_MESSAGE
	clear
