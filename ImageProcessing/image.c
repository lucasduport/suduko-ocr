#include "tools.h"
#include "image.h"
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

uc *copyPixels(uc *pixels, st len) {
	uc *newPixels = malloc(sizeof(uc) * len);
	if (newPixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (st i = 0; i < len; i++) newPixels[i] = pixels[i];
	return newPixels;
}

Image *newImage(st width, st height) {
	Image *image = (Image *)malloc(sizeof(Image));
	if (image == NULL) errx(EXIT_FAILURE, "malloc failed");
	image->pixels = (uc *)malloc(width * height * sizeof(uc));
	if (image->pixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	image->width = width;
	image->height = height;
	return image;
}

Image *copyImage(Image *image) {
	Image *copy = malloc(sizeof(Image));
	if (copy == NULL) errx(EXIT_FAILURE, "malloc failed");
	copy->width = image->width;
	copy->height = image->height;
	copy->pixels = copyPixels(image->pixels, image->width * image->height);
	return copy;
}

void freeImage(Image *image) {
	free(image->pixels);
	free(image);
}

Image *openImage(const char *filename) {
	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface
		= SDL_ConvertSurfaceFormat(surface_tmp, SDL_PIXELFORMAT_RGB888, 0);
	if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface_tmp);
	Uint32 *pxls = surface->pixels;
	Image *image = newImage(surface->w, surface->h);
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	if (SDL_LockSurface(surface) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
	for (st i = 0; i < len; i++) pixels[i] = pxls[i];
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return image;
}
