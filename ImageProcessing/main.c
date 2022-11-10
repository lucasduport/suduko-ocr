#include "display.h"
#include "hough.h"
#include "openImage.h"
#include "imageRGBA.h"
#include "tools.h"
#include "transformImage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
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

void init() {
	initTrig();
	if (SDL_Init(SDL_INIT_VIDEO) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
}

void printHelp(char *exeName) {
	printf("Usage: %s <command> <filename> [options]\n", exeName);
	printf("\t-h, --help\t\t\t"
		   "prints this help message\n");
	printf("\t-r, --rotate <image> <angle>\t"
		   "rotate the image <image> with the angle <angle>.\n");
	printf("\t-R, --rotateView <image>\t"
		   "rotate the image <image> with a preview (use arrow keys).\n");
	printf("\t-d, --demo <image>\t\t"
		   "see full demo.\n");
	printf("\t-t, --test <image> [options]\t"
		   "test the image <image> with the given options.\n");
}

int missingArg(char *exeName, char *command) {
	printf("Missing argument for %s.\n", command);
	printHelp(exeName);
	return 1;
}

void exeRotate(char *filename, int angle) {
	Image *image = openImage(filename);
	Image *rotated = rotateImage(image, angle, 0);
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	char destName[40];
	sprintf(destName, "%s_r%i.png", filenameStripped, angle);
	saveImage(rotated, destName);
	freeImage(rotated);
	freeImage(image);
}

void exeRotateView(char *filename) {
	Image *image = openImage(filename);
	int theta = rotateWithView(image);
	Image *rotated = rotateImage(image, theta, 0);
	freeImage(image);
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	char destName[40];
	sprintf(destName, "%s_r%i.png", filenameStripped, theta);
	saveImage(rotated, destName);
	freeImage(rotated);
}

void exeDemo(char *filename) {
	// open image
	Image *image = openImage(filename);
	autoResize(image, WINDOW_WIDTH, WINDOW_HEIGHT);
	// rotate image
	int theta = rotateWithView(image);
	Image *rotated = rotateImage(image, theta, 255);
	Image *copy = copyImage(rotated);
	freeImage(image);
	// preprocess image
	saturateImage(rotated);
	displayImage(rotated, "Saturated");
	// detect grid
	Quadri *quadri = detectGrid(rotated);
	if (quadri == NULL) {
		freeImage(rotated);
		errx(1, "No grid detected.");
	}
	// display results
	showQuadri(rotated, quadri, 0, 255, 0);
	Image *extracted = extractGrid(copy, quadri, 900, 900);
	freeImage(copy);
	freeImage(rotated);
	freeQuadri(quadri);
	thresholdCells(extracted);
	displayImage(extracted, "Extracted grid");
	// save image
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	saveBoard(extracted, filenameStripped);
	freeImage(extracted);
}

void exeTest(char *filename, int radius) {
	Image *image = openImage(filename);
	autoResize(image, WINDOW_WIDTH, WINDOW_HEIGHT);
	displayImage(image, "Original image");
	gaussianBlur(image);
	calibrateImage(image, radius);
	displayImage(image, "Saturated");
	sobelFilter(image);
	displayImage(image, "Sobel");
	freeImage(image);
}

void exeDigit(char *filename, char *digit_filename, int x, int y) {
	Image *bg = openImage(filename);
	ImageRGBA *digit = openImageRGBA(digit_filename);
	displayImage(bg, "Before");
	placeDigit(bg, digit, x, y);
	displayImage(bg, "After");
}

int main(int argc, char *argv[]) {
	char *exeName = argv[0];
	if (argc < 2) {
		printHelp(exeName);
		return 1;
	}
	init();
	for (int i = 1; i < argc; i++) {
		char *command = argv[i];
		if (!strcmp(command, "-h") || !strcmp(command, "--help")) {
			printHelp(exeName);
			return 0;
		} else if (!strcmp(command, "-r") || !strcmp(command, "--rotate")) {
			if (i + 2 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			int angle = atoi(argv[++i]);
			exeRotate(filename, angle);
		} else if (!strcmp(command, "-R") || !strcmp(command, "--rotateView")) {
			if (i + 1 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			exeRotateView(filename);
		} else if (!strcmp(command, "-d") || !strcmp(command, "--demo")) {
			if (i + 1 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			exeDemo(filename);
		} else if (!strcmp(command, "-t") || !strcmp(command, "--test")) {
			if (i + 4 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			char *digit_filename = argv[++i];
			int x = atoi(argv[++i]);
			int y = atoi(argv[++i]);
			exeDigit(filename, digit_filename, x, y);
		} else {
			printf("Unknown command %s.\n", command);
			printHelp(exeName);
			return 1;
		}
	}
	IMG_Quit();
	SDL_Quit();
	return 0;
}
