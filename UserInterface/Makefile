.SILENT: all
.SILENT: run
.SILENT: clean
.SILENT: save
.SILENT: format

SRC = *.c ../ImageProcessing/*.c

OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
all:
	
	clang -Wall -Wextra -std=c99 -O3 \
	-MMD ${SRC} -c -g `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl2` -fsanitize=address
	clang *.o -o main `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_image -lm -fsanitize=address -g

clean:
	rm -rf *.[do] main *.out
	rm -rf COMMIT_MESSAGE
	rm -rf saved_images/
	rm -rf tmp/
	clear

save:
	make clean
	echo "Enter commit message (optional):"
	read var; \
	echo $$var > COMMIT_MESSAGE
	git add --all
	git status --porcelain >> COMMIT_MESSAGE
	git commit -F COMMIT_MESSAGE
	
format:
	clang-format --style=.clang-format -i *.[ch]