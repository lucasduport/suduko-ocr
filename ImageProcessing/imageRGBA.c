#include "imageRGBA.h"
#include "matrices.h"
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Contains basics functions for manipulating the ImageRGBA struct

/*
Pixel *copyPixelsRGBA(Pixel *pixels, st len) {
	Pixel *newPixels = malloc(sizeof(Pixel) * len);
	if (newPixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (st i = 0; i < len; i++) newPixels[i] = pixels[i];
	return newPixels;
}
*/
#if 0
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

void createAlpha(ImageRGBA *image, int min, int max) {
	Pixel *pxls = image->pixels;
	st len = image->width * image->height;
	Pixel pxl;
	int val;
	for (st i = 0; i < len; i++) {
		pxl = pxls[i];
		val = (pxl.r + pxl.g + pxl.b) / 3;
		if (val < min || val > max)
			pxls[i].a = 0;
	}
}

void placeDigit(Image *bg, ImageRGBA *digit, Quad *grid, int i, int j) {
	float mat[3][3];
	getTransformMatrix(grid, 9 * 384, 9 * 384, mat);
	float input[3];
	input[2] = 1;
	float res[3];
	int w = bg->width, h = bg->height;
	uc *pxls = bg->pixels;
	Pixel *d_pxls = digit->pixels;
	Pixel pxl;
	int val;
	int x, y;
	for (int d_y = 0; d_y < 256; d_y++) {
		for (int d_x = 0; d_x < 256; d_x++) {
			input[0] = d_x + 384 * i + 64;
			input[1] = d_y + 384 * j + 64;
			matMul33_31(mat, input, res);
			x = res[0] / res[2];
			y = res[1] / res[2];
			if (x < 0 || x >= w || y < 0 || y >= h) continue;
			pxl = d_pxls[d_y * 256 + d_x];
			val = (pxl.r + pxl.g + pxl.b) * pxl.a / 3;
			val += (255 - pxl.a) * pxls[y * w + x];
			pxls[y * w + x] = val / 255;
		}
	}
}
#endif // 0
