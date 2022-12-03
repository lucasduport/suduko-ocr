#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <string.h>
#include "../ImageProcessing/display.h"
#include "../ImageProcessing/filters.h"
#include "../ImageProcessing/hough.h"
#include "../ImageProcessing/saveImage.h"
#include "../ImageProcessing/tools.h"
#include "../ImageProcessing/transformImage.h"
#include "../ImageProcessing/cellExtraction.h"
#include "../ImageProcessing/cellsDetection.h"

void init()
{
	initTrig();
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
}

int **readSudoku(const char *filename)
{
	int **array = malloc(9 * sizeof(int *));
	for (int i = 0; i < 9; i++)
		array[i] = (int *)malloc(9 * sizeof(int));
	if (!array)
		return NULL;
	FILE *file = fopen(filename, "r");
	if (!file)
		errx(1, "Couldn't open file \"%s\"", filename);
	size_t count;
	char buffer = '0';
	int value;
	size_t i = 0;
	size_t j = 0;

	do
	{
		count = fread(&buffer, 1, 1, file);
		if (buffer == '.')
			value = 0;
		else if (buffer >= '0' && buffer <= '9')
			value = buffer - '0';
		else
		{
			if (buffer == '\n' && j)
			{
				i++;
				j = 0;
			}
			continue;
		}
		array[i][j] = value;
		j++;
	} while (count != 0);
	fclose(file);
	return array;
}

void searchDigit(int **sudoku, int n, int *i, int *j)
{
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			if (sudoku[y][x] == n)
			{
				*i = x;
				*j = y;
				return;
			}
		}
	}
	*i = -1;
	*j = -1;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		errx(1, "Usage: %s <image>", argv[0]);
	char *filename = argv[1];
	
	init();

	Image *final = openImage(filename, 4);
	autoResize(final, WINDOW_WIDTH, WINDOW_HEIGHT);
	Image *image = copyImage(final);
	toGrey(image);
	Image *to_extract = copyImage(image);

	// preprocessing
	// displayImage(image, "Original image");
	gaussianBlur(image);
	calibrateImage(image, 200, 255);
	// displayImage(image, "Saturated");
	sobelFilter(image);
	// displayImage(image, "Sobel");
	gaussianBlur(image);
	thresholdToUpper(image, 16);
	// displayImage(image, "Calibrate");

	// detect grid
	Quad *quad = detectGrid(image);
	if (quad == NULL)
		errx(1, "No grid detected.");

	// display results
	showQuad(image, quad, 0, 255, 0);

	// extract grid
	// Image *extracted = extractGrid(to_extract, quad, new_size, new_size);
	Image *extracted = extractGrid(to_extract, quad, 1440, 1440);
	invertImage(extracted);
	displayImage(extracted, "Extracted grid");
	freeImage(to_extract);

	// extract cells
	int *coords_x;
	int *coords_y;
	int nb_cells;
	nb_cells = getGridDimension(extracted, &coords_x, &coords_y);
	int cell_size = 1440 / nb_cells;
	int border_size = cell_size / 10;
	for (int i = 0; i < nb_cells + 1; i++)
	{
		coords_x[i] += border_size;
		coords_y[i] += border_size;
	}
	char filename_stripped[30];
	cleanPath(filename, filename_stripped);
	// saveCells(extracted, CELLSIZE, 5, filenameStripped);
	saveCells(extracted, border_size, coords_x, coords_y, filename_stripped);
	free(coords_x);
	free(coords_y);
	freeImage(extracted);

	//TODO: use neural network
	int **sudoku = readSudoku("../Solver/grid_00");
	int **solved = readSudoku("../Solver/grid_00.result");
	int a = 10, b = 11, c = 12, d = 13, e = 14, f = 15, n = 16;
	int _hexa[16][16] = {
		{7, 0, e, 0, a, 0, 3, 0, 0, 2, 0, 9, 0, n, 5, b},
		{4, 0, c, 6, e, 2, 0, n, d, 5, 0, 3, a, 0, f, 1},
		{f, 2, 9, 0, 0, 0, 5, b, e, 0, 0, 0, 0, 0, 0, d},
		{0, 0, 0, 3, c, 8, 7, d, b, 0, 0, a, 6, 0, 0, 2},
		{8, 3, 6, 1, 5, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, c},
		{0, f, 0, 9, 0, a, 0, 0, 0, 8, 0, d, 1, 2, 0, 0},
		{d, 0, 2, 0, 1, e, n, 9, 0, 0, 0, f, 0, 0, 0, 0},
		{e, 4, 0, 0, 0, 7, 2, 1, 0, 0, 6, c, 5, f, 0, 0},
		{9, 6, 0, n, 0, 0, 0, 5, f, 0, 2, 0, c, 0, a, 0},
		{c, 1, 0, 7, 0, 0, 0, 0, 9, d, 0, e, 0, 4, 0, n},
		{0, 0, 0, 0, f, 9, d, 0, 0, n, 4, 7, 0, 0, 0, 0},
		{1, 0, d, 0, 2, n, b, f, 0, 0, 9, 0, 0, 0, 7, 5},
		{0, 0, n, 0, 0, 0, 1, 0, 0, 0, 0, b, d, e, 0, a},
		{2, e, 7, 0, 9, 0, a, 8, n, 0, 0, 5, b, c, 6, 4},
		{b, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 9, 1, 0, 0},
	};
	int _hexa_solved[16][16] = {
		{7, d, e, 8, a, f, 3, 6, 1, 2, c, 9, 4, n, 5, b},
		{4, b, c, 6, e, 2, 9, n, d, 5, 7, 3, a, 8, f, 1},
		{f, 2, 9, a, 4, 1, 5, b, e, 6, n, 8, 3, 7, c, d},
		{5, n, 1, 3, c, 8, 7, d, b, 4, f, a, 6, 9, e, 2},
		{8, 3, 6, 1, 5, b, 4, 7, 2, c, e, n, f, a, d, 9},
		{a, 7, 4, e, 8, d, f, 2, 5, 9, 1, 6, n, b, 3, c},
		{n, f, 5, 9, 6, a, c, 3, 7, 8, b, d, 1, 2, 4, e},
		{d, c, 2, b, 1, e, n, 9, 4, a, 3, f, 7, 5, 8, 6},
		{e, 4, a, d, n, 7, 2, 1, 8, b, 6, c, 5, f, 9, 3},
		{9, 6, 8, n, b, 4, e, 5, f, 3, 2, 1, c, d, a, 7},
		{c, 1, f, 7, 3, 6, 8, a, 9, d, 5, e, 2, 4, b, n},
		{3, 5, b, 2, f, 9, d, c, a, n, 4, 7, e, 6, 1, 8},
		{1, a, d, c, 2, n, b, f, 6, e, 9, 4, 8, 3, 7, 5},
		{6, 9, n, 5, 7, c, 1, 4, 3, f, 8, b, d, e, 2, a},
		{2, e, 7, f, 9, 3, a, 8, n, 1, d, 5, b, c, 6, 4},
		{b, 8, 3, 4, d, 5, 6, e, c, 7, a, 2, 9, 1, n, f},
	};
	int **hexa = (int **)malloc(16 * sizeof(int *));
	for (int i = 0; i < 16; i++)
	{
		hexa[i] = (int *)malloc(16 * sizeof(int));
		for (int j = 0; j < 16; j++)
		{
			hexa[i][j] = _hexa[i][j];
		}
	}
	int **hexa_solved = (int **)malloc(16 * sizeof(int *));
	for (int i = 0; i < 16; i++)
	{
		hexa_solved[i] = (int *)malloc(16 * sizeof(int));
		for (int j = 0; j < 16; j++)
		{
			hexa_solved[i][j] = _hexa_solved[i][j];
		}
	}

	char dirname[30];
	cleanPath(filename, dirname);
	// Image **digits = loadCells(sudoku, dirname);
	Image **digits = loadCells((int **)hexa, dirname);

	// puts numbers back in original image
	for (int j = 0; j < nb_cells; j++)
	{
		for (int i = 0; i < nb_cells; i++)
		{
			if (!hexa[j][i])
			{
				int n = hexa_solved[j][i];
				placeDigit(final, digits[n - 1], quad, i, j);
			}
		}
	}
	displayImage(final, "With numbers placed");

	freeQuad(quad);
	freeImage(image);
	freeImage(final);
	for (int i = 0; i < 9; i++)
	{
		free(sudoku[i]);
		free(solved[i]);
	}
	for (int i = 0; i < nb_cells; i++)
	{
		// free(sudoku[i]);
		free(hexa[i]);
		// free(solved[i]);
		free(hexa_solved[i]);
		freeImage(digits[i]);
	}
	free(sudoku);
	free(solved);
	free(hexa);
	free(hexa_solved);
	free(digits);
}
