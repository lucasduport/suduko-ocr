#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "image.h"
#include "matrices.h"

// Contains basics functions for manipulating the Image struct

uc *copyChannel(uc *channel, st len)
{
	uc *new_channel = (uc *)malloc(sizeof(uc) * len);
	if (new_channel == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	for (st i = 0; i < len; i++)
		new_channel[i] = channel[i];
	return new_channel;
}

Image *newImage(uc nb_channels, st width, st height)
{
	Image *image = (Image *)malloc(sizeof(Image));
	if (image == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	image->nb_channels = nb_channels;
	image->width = width;
	image->height = height;
	uc **channels = (uc **)malloc(nb_channels * sizeof(uc *));
	if (channels == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	uc *channel;
	for (uc i = 0; i < nb_channels; i++)
	{
		channel = (uc *)malloc(width * height * sizeof(uc));
		if (channel == NULL)
			errx(EXIT_FAILURE, "malloc failed");
		channels[i] = channel;
	}
	image->channels = channels;
	return image;
}

Image *copyImage(Image *image)
{
	Image *copy = (Image *)malloc(sizeof(Image));
	if (copy == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	uc nb_channels = image->nb_channels;
	copy->nb_channels = nb_channels;
	st width = image->width, height = image->height;
	copy->width = width;
	copy->height = height;
	uc **channels = (uc **)malloc(nb_channels * sizeof(uc *));
	if (channels == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	for (uc i = 0; i < nb_channels; i++)
		channels[i] = copyChannel(image->channels[i], width * height);
	copy->channels = channels;
	return copy;
}

void freeImage(Image *image)
{
	for (uc i = 0; i < image->nb_channels; i++)
		free(image->channels[i]);
	free(image->channels);
	free(image);
}

void surfaceToGrey(SDL_Surface *surface, Image *image)
{
	st len = surface->w * surface->h;
	SDL_PixelFormat *format = surface->format;
	Uint32 *pixels = surface->pixels;
	uc *grey_channel = image->channels[0];
	uc r, g, b, a;
	for (st i = 0; i < len; i++)
	{
		SDL_GetRGBA(pixels[i], format, &r, &g, &b, &a);
		grey_channel[i] = (r + g + b) / 3;
	}
}

void surfaceToRGBA(SDL_Surface *surface, Image *image)
{
	st len = surface->w * surface->h;
	SDL_PixelFormat *format = surface->format;
	Uint32 *pixels = surface->pixels;
	uc *r_channel = image->channels[0];
	uc *g_channel = image->channels[1];
	uc *b_channel = image->channels[2];
	uc *a_channel = image->channels[3];
	uc r, g, b, a;
	for (st i = 0; i < len; i++)
	{
		SDL_GetRGBA(pixels[i], format, &r, &g, &b, &a);
		r_channel[i] = r;
		g_channel[i] = g;
		b_channel[i] = b;
		a_channel[i] = a;
	}
}

Image *openImage(const char *filename, uc nb_channels)
{
	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface
		= SDL_ConvertSurfaceFormat(surface_tmp, SDL_PIXELFORMAT_RGBA8888, 0);
	if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface_tmp);
	Image *image = newImage(nb_channels, surface->w, surface->h);
	if (SDL_LockSurface(surface) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	switch (nb_channels)
	{
		case 1:
			surfaceToGrey(surface, image);
			break;
		case 4:
			surfaceToRGBA(surface, image);
			break;
		default:
			errx(EXIT_FAILURE, "an Image should have 1 or 4 channels");
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return image;
}

void placeGrey(Image *bg, Image *digit, float mat[3][3], int i, int j)
{
	int w = bg->width, h = bg->height;
	float input[3] = {0, 0, 1};
	float res[3];
	int x, y;
	uc *b_channel = bg->channels[0];
	uc *d_channel = digit->channels[0];
	for (int d_y = 0; d_y < 256; d_y++)
	{
		input[1] = d_y + 384 * j + 64;
		for (int d_x = 0; d_x < 256; d_x++)
		{
			input[0] = d_x + 384 * i + 64;
			matMul33_31(mat, input, res);
			x = res[0] / res[2];
			y = res[1] / res[2];
			if (x < 0 || x >= w || y < 0 || y >= h)
				continue;
			b_channel[y * w + x] = d_channel[d_y * 256 + d_x];
		}
	}
}

void placeRGBA(Image *bg, Image *digit, float mat[3][3], int i, int j)
{
	int w = bg->width, h = bg->height;
	uc *b_a_channel = bg->channels[3];
	uc *d_a_channel = digit->channels[3];
	float input[3] = {0, 0, 1};
	float res[3];
	int x, y;
	int val;
	for (uc n = 0; n < 4; n++)
	{
		uc *b_channel = bg->channels[n];
		uc *d_channel = digit->channels[n];
		for (int d_y = 0; d_y < 256; d_y++)
		{
			input[1] = d_y + 384 * j + 64;
			for (int d_x = 0; d_x < 256; d_x++)
			{
				input[0] = d_x + 384 * i + 64;
				matMul33_31(mat, input, res);
				x = res[0] / res[2];
				y = res[1] / res[2];
				if (x < 0 || x >= w || y < 0 || y >= h)
					continue;
				if (n < 4)
				{ // RGB channels
					uc d_pxl = d_channel[d_y * 256 + d_x];
					uc d_a = d_a_channel[d_y * 256 + d_x];
					uc b_pxl = b_channel[y * w + x];
					uc b_a = b_a_channel[y * w + x];
					val = d_pxl * d_a * 255 + b_pxl * b_a * (255 - d_a);
					val /= d_a * 255 + b_a * (255 - d_a);
					b_channel[y * w + x] = val;
				}
				else
				{ // alpha channel
					uc d_a = d_channel[d_y * 256 + d_x];
					uc b_a = b_channel[y * w + x];
					val = d_a * 255 + b_a * (255 - d_a);
					b_channel[y * w + x] = val / 255;
				}
			}
		}
	}
}

void placeDigit(Image *background, Image *digit, Quad *grid, int i, int j)
{
	uc nb_channels = background->nb_channels;
	if (digit->nb_channels != nb_channels)
		errx(EXIT_FAILURE, "background and digit must have the same number of "
						   "channels");
	float mat[3][3];
	getTransformMatrix(grid, 9 * 384, 9 * 384, mat);
	switch (nb_channels)
	{
		case 1:
			placeGrey(background, digit, mat, i, j);
			break;
		case 4:
			placeRGBA(background, digit, mat, i, j);
			break;
		default:
			errx(EXIT_FAILURE, "Image must have 1 or 4 channels");
	}
}
