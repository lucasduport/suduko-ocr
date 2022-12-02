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
	Image *extracted = extractGrid(to_extract, quad, 9 * CELLSIZE, 9 * CELLSIZE);
	freeImage(to_extract);
	invertImage(extracted);
	displayImage(extracted, "Extracted grid");

	// save image
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	//saveBoard(extracted, filenameStripped);
	saveCells(extracted, CELLSIZE, 5, filenameStripped);
	freeImage(extracted);

	//TODO: use neural network
	int **sudoku = readSudoku("../Solver/grid_00");
	int **solved = readSudoku("../Solver/grid_00.result");

	char dirname[30];
	cleanPath(filename, dirname);
	Image **digits = loadCells(sudoku, dirname);

	// puts numbers back in original image
	for (int j = 0; j < 9; j++)
	{
		for (int i = 0; i < 9; i++)
		{
			if (!sudoku[j][i])
			{
				int n = solved[j][i];
				placeDigit(final, digits[n - 1], quad, i, j);
			}
		}
	}
	displayImage(final, "With numbers placed");

	freeQuad(quad);
	freeImage(image);
	freeImage(final);
	for (st i = 0; i < 9; i++)
	{
		free(sudoku[i]);
		free(solved[i]);
		freeImage(digits[i]);
	}
	free(sudoku);
	free(solved);
	free(digits);
}
