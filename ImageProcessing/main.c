#include "display.h"
#include "hough.h"
#include "matrices.h"
#include "openImage.h"
#include "tools.h"
#include "transformImage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

char *cleanPath(char *filename, char *dest) {
	char *slash = strrchr(filename, '/');
	if (slash == NULL) {
		strcpy(dest, filename);
	} else {
		strcpy(dest, slash + 1);
	}
	char *dot = strrchr(dest, '.');
	if (!dot || dot == dest) return dest;
	*dot = '\0';
	return dest;
}

int main(int argc, char *argv[]) {
	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		printf("--demo <image>: see full demo.\n");
		printf("--rotateView <image>: rotate the image <image> with a preview "
			   "(use arrow keys).\n");
		printf("--rotate <angle> <image> : rotate the image <image> with "
			   "<angle> angle.\n");
		return 0;
	}
	if (argc == 3) {
		if (strcmp(argv[1], "--rotateView") == 0) {
			if (strcmp(argv[2], "--help") == 0) {
				printf("Usage: ./main --rotateView <image>\n");
				return 0;
			}
			Image *image = openImage(argv[2]);
			int theta = rotateWithView(image);
			Image *rotated = rotateImage(image, theta, 0);
			char filenameWE[30];
			cleanPath(argv[2], filenameWE);
			char destName[40];
			sprintf(destName, "%s_r%i.png", filenameWE, theta);
			saveImage(rotated, destName);
			freeImage(rotated);
			freeImage(image);
			return 0;
		}
		if (strcmp(argv[1], "--demo") == 0) {
			if (strcmp(argv[2], "--help") == 0) {
				printf("Usage: ./main --demo <image>\n");
				return 0;
			}
			Image *image = openImage(argv[2]);
			Image *resized = autoResize(image, WINDOW_WIDTH, WINDOW_HEIGHT);
			freeImage(image);
			int theta = rotateWithView(resized);
			Image *rotated = rotateImage(resized, theta, 255);
			freeImage(resized);
			saturateImage(rotated);
			Quadri *quadri = detectGrid(rotated);
			if (quadri == NULL) {
				printf("No grid detected\n");
				freeImage(rotated);
				return 1;
			}
			showQuadri(rotated, quadri, 0, 255, 0);
			Image *extracted = extractGrid(rotated, quadri, 900, 900);
			displayImage(extracted, "Extracted grid");
			char filenameWE[30];
			cleanPath(argv[2], filenameWE);
			saveBoard(extracted, filenameWE);
			freeImage(extracted);
			freeQuadri(quadri);
			freeImage(rotated);
			return 0;
		}
	}
	if (argc == 4) {
		if (strcmp(argv[1], "--rotate") == 0) {
			if (strcmp(argv[2], "--help") == 0) {
				printf("Usage: ./main --rotate <angle> <image>\n");
				return 0;
			}
			Image *image = openImage(argv[3]);
			Image *rotated = rotateImage(image, atoi(argv[2]), 0);
			char filenameWE[30];
			cleanPath(argv[3], filenameWE);
			char destName[40];
			sprintf(destName, "%s_r%i.png", filenameWE, atoi(argv[2]));
			saveImage(rotated, destName);
			freeImage(rotated);
			freeImage(image);
			return 0;
		}
	}
	printf("Try: ./main --help\n");
	return 0;
}
