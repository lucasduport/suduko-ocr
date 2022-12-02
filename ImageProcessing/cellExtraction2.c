#include "cellExtraction.h"
#include "filters.h"
#include "display.h"
#include "transformImage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/stat.h>
#include <err.h>

void addBorders(Image *image, int border_size, uc bg)
{
	int w = image->width, h = image->height;
	int new_w = w + 2 * border_size, new_h = h + 2 * border_size;
	uc nb_channels = image->nb_channels;
	for (uc n = 0; n < nb_channels; n++)
	{
		uc *channel = image->channels[n];
		uc *new_channel = malloc(new_w * new_h * sizeof(uc));
		if (new_channel == NULL)
			errx(EXIT_FAILURE, "malloc failed");
		for (int y = 0; y < new_h; y++)
		{
			if (y < border_size || y >= new_h - border_size)
			{
				for (int x = 0; x < new_w; x++)
					new_channel[y * new_w + x] = bg;
				continue;
			}
			for (int x = 0; x < new_w; x++)
			{
				if (x < border_size || x >= new_w - border_size)
					new_channel[y * new_w + x] = bg;
				else
					new_channel[y * new_w + x] = channel[(y - border_size) * w + x - border_size];
			}
		}
		free(channel);
		image->channels[n] = new_channel;
	}
	image->width = new_w;
	image->height = new_h;
}

Image *getCellWithBorders(Image *image, int cell_size, int border_size, int i, int j)
{
	st w = image->width;
	uc nb_channels = image->nb_channels;
	st cell_w = cell_size + 2 * border_size;
	st cell_h = cell_size + 2 * border_size;
	Image *cell = newImage(nb_channels, cell_w, cell_h);
	st x0 = i * cell_size;
	st y0 = j * cell_size;
	for (uc n = 0; n < nb_channels; n++)
	{
		uc *channel = image->channels[n];
		uc *cell_channel = cell->channels[n];
		for (st y = 0; y < cell_h; y++)
			for (st x = 0; x < cell_w; x++)
				cell_channel[y * cell_w + x] = channel[(y0 + y) * w + x0 + x];
	}
	return cell;
}

void removeBorders(Image *image, int cell_size)
{
	int w = image->width;
	int border_size = (w - cell_size) / 2;
	uc nb_channels = image->nb_channels;
	int x0, y0;
	for (uc n = 0; n < nb_channels; n++)
	{
		uc *channel = image->channels[n];
		uc *new_channel = malloc(cell_size * cell_size * sizeof(uc));
		if (new_channel == NULL)
			errx(EXIT_FAILURE, "malloc failed");
		for (int y = 0; y < cell_size; y++)
		{
			y0 = y + border_size;
			for (int x = 0; x < cell_size; x++)
			{
				x0 = x + border_size;
				new_channel[y * cell_size + x] = channel[y0 * w + x0];
			}
		}
		free(channel);
		image->channels[n] = new_channel;
	}
	image->width = cell_size;
	image->height = cell_size;
}

void saveCell(Image *image, int *coords_x, int *coords_y, int border_size, const char *dirname, int i, int j)
{
	int x_min = coords_x[i];
	int x_max = coords_x[i + 1];
	int y_min = coords_y[i];
	int y_max = coords_y[i + 1];
	Image *cell = getCellWithBorders(image, cell_size, border_size, i, j);
	calibrateCell(cell);
	removeBorders(cell, 28);
	SDL_Surface *surface = imageToSurface(cell);
	st len = strlen(dirname) + 9;
	char filename[len];
	snprintf(filename, len, "%s/%d_%d.png", dirname, i+1, j+1);
	if (IMG_SavePNG(surface, filename) != 0)
	{
		errx(EXIT_FAILURE, "Error while saving image");
	}
	freeImage(cell);
	SDL_FreeSurface(surface);
}

void saveCells(Image *image, int nb_cells, int border_size, int *coords_x,
		int *coords_y, const char *filename)
{
	addBorders(image, border_size, 127);
	st len = strlen(filename) + 7;
	char dirname[len];
	snprintf(dirname, len, "board_%s", filename);
	struct stat st_ = {0};
	if (stat(dirname, &st_) == -1)
	{
		mkdir(dirname, 0700);
	}
	for (int i = 0; i < nb_cells; i++)
	{
		for (int j = 0; j < nb_cells; j++)
		{
			saveCell(image, coords_x, coords_y, border_size, dirname, i, j);
		}
	}
}

void loadDefaultCells(Image **cells, char *dirname)
{
	for (int i = 0; i < 9; i++) {
		if (cells[i] == NULL)
		{
			st len = strlen(dirname) + 8;
			char filename[len];
			snprintf(filename, len, "%s/%d.png", dirname, i+1);
			Image *image = openImage(filename, 4);
			createAlpha(image, 0, 191);
			toColor(image, 0, 255, 0);
			cells[i] = image;
		}
	}
}


Image **loadCells(int **grid, char *dirname)
{
	Image **digits = (Image **)malloc(9 * sizeof(Image *));
	if (digits == NULL)
		errx(EXIT_FAILURE, "malloc failed");
	for (int i = 0; i < 9; i++)
		digits[i] = NULL;
	st n;
	for (st j = 0; j < 9; j++)
	{
		for (st i = 0; i < 9; i++)
		{
			n = grid[j][i];
			if (n == 0)
				continue;
			if (digits[n - 1])
				continue;
			st len = strlen(dirname) + 17;
			char filename[len];
			snprintf(filename, len, "board_%s/%zu_%zu.png", dirname, i+1, j+1);
			Image *image = openImage(filename, 4);
			resizeImage(image, 256, 256);
			invertImage(image);
			createAlpha(image, 0, 191);
			toColor(image, 0, 255, 0);
			digits[n - 1] = image;
		}
	}
	loadDefaultCells(digits, "../ImageProcessing/Numbers");
	return digits;
}

void cleanPath(char *filename, char *dest)
{
	char *slash = strrchr(filename, '/');
	if (slash == NULL)
	{
		strcpy(dest, filename);
	}
	else
	{
		strcpy(dest, slash + 1);
	}
	char *dot = strrchr(dest, '.');
	if (dot && dot != dest)
		*dot = '\0';
}
