#include "imageRGBA.h"
#include "tools.h"
#include "iamge.h"
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Pixel *copyPixelsRGBA(Pixel *pixels, st len) {
	Pixel *newPixels = malloc(sizeof(Pixel) * len);
	if (newPixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (st i = 0; i < len; i++) newPixels[i] = pixels[i];
	return newPixels;
}

ImageRGBA *newImageRGBA(st width, st height) {
	ImageRGBA *image = (ImageRGBA *)malloc(sizeof(ImageRGBA));
	if (image == NULL) errx(EXIT_FAILURE, "malloc failed");
	image->pixels = (Pixel *)malloc(width * height * sizeof(Pixel));
	if (image->pixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	image->width = width;
	image->height = height;
	return image;
}

ImageRGBA *copyImageRGBA(ImageRGBA *image) {
	ImageRGBA *copy = malloc(sizeof(ImageRGBA));
	if (copy == NULL) errx(EXIT_FAILURE, "malloc failed");
	copy->width = image->width;
	copy->height = image->height;
	copy->pixels = copyPixelsRGBA(image->pixels, image->width * image->height);
	return copy;
}

void freeImageRGBA(ImageRGBA *image) {
	free(image->pixels);
	free(image);
}

ImageRGBA *openImageRGBA(const char *filename) {
	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface
		= SDL_ConvertSurfaceFormat(surface_tmp, SDL_PIXELFORMAT_RGBA8888, 0);
	if (surface == NULL) errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface_tmp);
	Uint32 *pxls = surface->pixels;
	SDL_PixelFormat *format = surface->format;
	ImageRGBA *image = newImageRGBA(surface->w, surface->h);
	Pixel *pixels = image->pixels;
	st len = image->width * image->height;
	if (SDL_LockSurface(surface) != 0) errx(EXIT_FAILURE, "%s", SDL_GetError());
	Uint8 r, g, b, a;
	for (st i = 0; i < len; i++) {
		SDL_GetRGBA(pxls[i], format, &r, &g, &b, &a);
		pixels[i] = (Pixel){r, g, b, a};
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return image;
}

// not sure about using it
Image *removeAlpha(ImageRGBA *image_rgba, uc threshold) {
	// replace high alpha by white
	st width = image_rgba->width, height = image_rgba->height;
	Pixel *pixels_rgba = image_rgba->pixels;
	Image *image = newImage(width, height);
	uc *pixels = image->pixels;
	st len = width * height;
	Pixel pixel;
	int val;
	for (st i = 0; i < len; i++) {
		pixel = pixels_rgba[i];
		if (pixel.a > threshold)
			val = 0;
		else {
			val = 255 - (pixel.r + pixel.g + pixel.b) / 3;
			val = val * pixel.a / 255;
		}
		pixels[i] = 255 - val;
	}
	return image;
}

void placeDigit(Image *image, ImageRGBA *digit, int x, int y, uc threshold) {
	int width = image->width, height = image->height;
	uc *pixels = image->pixels;
	Pixel *d_pixels = digit->pixels;
	// TODO: continune this
}
