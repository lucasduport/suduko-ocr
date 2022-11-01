#include "openImage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Image *openImage(const char *filename) {
	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface =
		SDL_ConvertSurfaceFormat(surface_tmp, SDL_PIXELFORMAT_RGB888, 0);
	if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface_tmp);
	Uint32 *pxls = surface->pixels;
	Image *image = newImage(surface->w, surface->h);
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	if (SDL_LockSurface(surface) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
	for (st i = 0; i < len; i++) { pixels[i] = pxls[i]; }
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return image;
}

void saveImage(Image *image, const char *filename) {
	SDL_Surface *surface = imageToSurface(image);
	char path[40];
	struct stat st = {0};
	if (stat("saved_images/", &st) == -1) mkdir("saved_images/", 0700);
	sprintf(path, "saved_images/%s", filename);
	if (IMG_SavePNG(surface, path) != 0) errx(1, "Error while saving image");
	printf("Image saved in %s\n", path);
	SDL_FreeSurface(surface);
	return;
}

void saveSquare(Image *image, const char *filename, Point *point, int size) {
	SDL_Surface *surface = imageToSurface(image);
	SDL_Rect rect = {point->x, point->y, size, size};
	SDL_Surface *cell = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);
	SDL_BlitSurface(surface, &rect, cell, NULL);
	if (IMG_SavePNG(cell, filename) != 0) {
		errx(1, "Error while saving image");
	}
	SDL_FreeSurface(cell);
	SDL_FreeSurface(surface);
	return;
}
// image must be an extracted board
// filename is the name of the folder to save the image to
void saveBoard(Image *image, const char *filename) {
	double percentageOfCell = 0.75; // 75% of the cell size
	struct stat st = {0};
	char dirname[40];
	sprintf(dirname, "board_%s", filename);
	if (stat(dirname, &st) == -1) { mkdir(dirname, 0700); }
	int size = image->width / 9;
	int effectiveSize = size * percentageOfCell;
	int gap = (size - effectiveSize) / 2;
	SDL_Surface *surface = imageToSurface(image);
	SDL_Surface *cell = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			SDL_Rect rect = {j * size + gap, i * size + gap, effectiveSize,
							 effectiveSize};
			SDL_BlitSurface(surface, &rect, cell, NULL);
			char name[40];
			sprintf(name, "%s/%d_%d.png", dirname, i + 1, j + 1);
			if (IMG_SavePNG(cell, name) != 0) {
				errx(1, "Error while saving image");
			}
		}
	}
	SDL_FreeSurface(cell);
	SDL_FreeSurface(surface);
	return;
}