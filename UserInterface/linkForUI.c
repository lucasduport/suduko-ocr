#include <sys/stat.h>
#include "linkForUI.h"

double *centerInput(const char *path)
{
	Image *cell = openImage(path, 1);
	autoCenter(cell, 15, 0);
	uc *channel = cell->channels[0];
	double *pixels = (double *)malloc(784 * sizeof(double));
	if (pixels == NULL)
	{
		errx(EXIT_FAILURE, "malloc failed");
	}
	for (int i = 0; i < 784; i++)
	{
		uc val = channel[i];
		pixels[i] = 2 * (double)val / 255 - 1;
	}
	freeImage(cell);
	return pixels;
}

void fastSolving(char *foldername)
{
	char ** filenames = getFilenamesInDir(foldername);
	if (filenames == NULL)
	{
		return;
	}
	struct stat st_ = {0};
	if (stat("fastSolved/", &st_) == -1)
		mkdir("fastSolved/", 448);
	st fileCount = 0;
	for(char *filename = filenames[fileCount]; filename != NULL; fileCount++, filename = filenames[fileCount])
	{
		printf("Fast solving %s\n", filename);
		Image *final = openImage(filename, 4);
		autoResize(final, IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT);

		Image *image = copyImage(final);
		toGrey(image);
		Image *to_extract = copyImage(image);

		gaussianBlur(image);
		calibrateImage(image, 150, 255);
		sobelFilter(image);
		gaussianBlur(image);
		thresholdToUpper(image, 24);

		Quad *quad = detectGrid(image);
		if (quad == NULL)
		{
			freeImage(final);
			freeImage(image);
			freeImage(to_extract);
			free(filename);
			continue;
		}

		Image *extracted = extractGrid(to_extract, quad, 1440, 1440);
		invertImage(extracted);
		freeImage(to_extract);
		int *coords_x;
		int *coords_y;
		int nb_cells;
		nb_cells = getGridDimension(extracted, &coords_x, &coords_y);
		int cell_size = 1440 / nb_cells;
		int border_size = cell_size / 5;
		for (int i = 0; i < nb_cells + 1; i++)
		{
			coords_x[i] += border_size;
			coords_y[i] += border_size;
		}
		char filename_stripped[30];
		cleanPath(filename, filename_stripped);
		saveCells(extracted, border_size, coords_x, coords_y, filename_stripped);
		freeImage(extracted);
		for (int i = 0; i < nb_cells + 1; i++)
		{
			coords_x[i] -= border_size;
			coords_x[i] *= nb_cells * 384;
			coords_x[i] /= 1440;
			coords_y[i] -= border_size;
			coords_y[i] *= nb_cells * 384;
			coords_y[i] /= 1440;
		}
		int **sudoku = (int **)malloc(nb_cells * sizeof(int *));
		for (int i = 0; i < nb_cells; i++)
		{
			sudoku[i] = (int *)malloc(nb_cells * sizeof(int));
		}
		Network *net = (Network *)malloc(sizeof(Network));
		Network_Load(net, "../NeuralNetwork/TrainedNetwork/NeuralNetData_3layers_OCR-Biased_100.0.dnn");
		float *results[nb_cells * nb_cells];
		for (int i = 0; i < nb_cells; i++)
		{
			for (int j = 0; j < nb_cells; j++)
			{
				char path[40];
				sprintf(path, "board_%s/%02d_%02d.png", filename_stripped, i + 1, j + 1);
				double *input = centerInput(path);
				results[nb_cells * j + i] = Network_Predict(net, input, 784);
				free(input);
				int imax = 0;
				float max = 0;
				for (int k = 0; k < 10; k++)
				{
					float proba = results[j * nb_cells + i][k];
					if (proba >= max)
					{
						max = proba;
						imax = k;
					}
				}
				sudoku[j][i] = imax;
				free(results[nb_cells * j + i]);
			}
		}
		Network_Purge(net);
		int **solved = (int **)malloc(nb_cells * sizeof(int *));
		for (int i = 0; i < nb_cells; i++)
		{
			solved[i] = (int *)malloc(nb_cells * sizeof(int));
			for (int j = 0; j < nb_cells; j++)
			{
				solved[i][j] = sudoku[i][j];
			}
		}
		switch (nb_cells)
		{
		case 9:
			solver(solved);
			break;
		//case 16:
			//solver16(solved);
		default:
			freeImage(final);
			freeImage(image);
			free(filename);
			freeQuad(quad);
			free(coords_x);
			free(coords_y);
			for (int i = 0; i < nb_cells; i++)
			{
				free(sudoku[i]);
				free(solved[i]);
			}
			free(sudoku);
			free(solved);
			char path[50];
			sprintf(path, "board_%s", filename_stripped);
			rmDir(path);
			continue;
		}
		char dirname[30];
		cleanPath(filename, dirname);
		Image **digits = loadCells(sudoku, dirname);
		for (int i = 0; i < nb_cells; i++)
		{
			Image *cell = digits[i];
			toRGBA(cell);
			for (uc n = 0; n < 4; n++)
			{
				uc *channel = cell->channels[n];
				uc *new_channel = newChannel(384 * 384);
				for (int i = 0; i < 384 * 384; i++)
				{
					new_channel[i] = 0;
				}
				for (int y = 0; y < 256; y++)
				{
					for (int x = 0; x < 256; x++)
					{
						new_channel[(y + 64) * 384 + (x + 64)] = channel[y * 256 + x];
					}
				}
				free(channel);
				cell->channels[n] = new_channel;
			}
			cell->width = 384;
			cell->height = 384;
		}
		for (int j = 0; j < nb_cells; j++)
		{
			for (int i = 0; i < nb_cells; i++)
			{
				if (!sudoku[j][i] && solved[j][i])
				{
					int n = solved[j][i];
					placeDigit(final, digits[n - 1], quad, coords_x[i], coords_y[j]);
				}
			}
		}
		free(coords_x);
		free(coords_y);
		
		char saveName[50];
		sprintf(saveName, "fastSolved/%s_solved.png", filename_stripped);
		autoResize(final, WINDOW_WIDTH * IMAGE_RATIO, WINDOW_HEIGHT * IMAGE_RATIO);
		saveImage(final, saveName);
		printf("Saved to %s\n", saveName);

		//remove saved cells (not needed when fastSvolving)
		sprintf(saveName, "board_%s", filename_stripped);
		rmDir(saveName);
		freeQuad(quad);
		freeImage(image);
		freeImage(final);
		for (int i = 0; i < nb_cells; i++)
		{
			free(sudoku[i]);
			free(solved[i]);
			freeImage(digits[i]);
		}
		free(sudoku);
		free(solved);
		free(digits);
		free(filename);
	}
	free(filenames);
}

Image* getSolvedImage(Menu *menu)
{
	char *filename = menu->originPath;

	Image *final = copyImage(menu->originImage);
	autoResize(final, IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT);

	Image *image = copyImage(final);
	toGrey(image);
	Image *to_extract = copyImage(image);

	// displayImage(image, "Original image");
	gaussianBlur(image);
	calibrateImage(image, 150, 255);
	// displayImage(image, "Saturated");
	sobelFilter(image);
	// displayImage(image, "Sobel");
	gaussianBlur(image);
	thresholdToUpper(image, 24);
	// displayImage(image, "Calibrate");

	Quad *quad = detectGrid(image);
	if (quad == NULL)
		displayColoredText(menu->filters_warn_label, "No grid detected", "red");

	// display results
	//showQuad(image, quad, 0, 255, 0);

	Image *extracted = extractGrid(to_extract, quad, 1440, 1440);
	invertImage(extracted);
	//displayImage(extracted, "Extracted grid");
	freeImage(to_extract);

	int *coords_x;
	int *coords_y;
	int nb_cells;
	nb_cells = getGridDimension(extracted, &coords_x, &coords_y);
	int cell_size = 1440 / nb_cells;
	int border_size = cell_size / 5;
	for (int i = 0; i < nb_cells + 1; i++)
	{
		coords_x[i] += border_size;
		coords_y[i] += border_size;
	}
	char filename_stripped[30];
	cleanPath(filename, filename_stripped);
	saveCells(extracted, border_size, coords_x, coords_y, filename_stripped);
	freeImage(extracted);
	for (int i = 0; i < nb_cells + 1; i++)
	{
		coords_x[i] -= border_size;
		coords_x[i] *= nb_cells * 384;
		coords_x[i] /= 1440;
		coords_y[i] -= border_size;
		coords_y[i] *= nb_cells * 384;
		coords_y[i] /= 1440;
	}
	int **sudoku = (int **)malloc(nb_cells * sizeof(int *));
	for (int i = 0; i < nb_cells; i++)
	{
		sudoku[i] = (int *)malloc(nb_cells * sizeof(int));
	}
	Network *net = (Network *)malloc(sizeof(Network));
	Network_Load(net, "../NeuralNetwork/TrainedNetwork/NeuralNetData_3layers_OCR-Biased_100.0.dnn");
	//Network_Display(net, 0);
	float *results[nb_cells * nb_cells];
	for (int i = 0; i < nb_cells; i++)
	{
		for (int j = 0; j < nb_cells; j++)
		{
			char path[40];
			sprintf(path, "board_%s/%02d_%02d.png", filename_stripped, i + 1, j + 1);
			double *input = centerInput(path);
			results[nb_cells * j + i] = Network_Predict(net, input, 784);
			free(input);
			int imax = 0;
			float max = 0;
			//printf("[");
			for (int k = 0; k < 10; k++)
			{
				float proba = results[j * nb_cells + i][k];
				if (proba >= max)
				{
					max = proba;
					imax = k;
				}
				//printf("%.3f ", proba);
			}
			//puts("]");
			//printf("digit = %d\n", imax);
			sudoku[j][i] = imax;
			free(results[nb_cells * j + i]);
		}
	}
	Network_Purge(net);
	/* DISPLAY SUDOKU
	int block_size = nb_cells == 9 ? 3 : 4;
	for (int i = 0; i < nb_cells; i++)
	{
		for (int j = 0; j < nb_cells; j++)
		{
			printf("%02d ", sudoku[i][j]);
			if ((j + 1) % block_size == 0)
				printf(" ");
		}
		printf("\n");
		if ((i + 1) % block_size == 0)
			printf("\n");
	}
	*/
	int **solved = (int **)malloc(nb_cells * sizeof(int *));
	for (int i = 0; i < nb_cells; i++)
	{
		solved[i] = (int *)malloc(nb_cells * sizeof(int));
		for (int j = 0; j < nb_cells; j++)
		{
			solved[i][j] = sudoku[i][j];
		}
	}
	switch (nb_cells)
	{
	case 9:
		solver(solved);
		break;
	case 16:
		solver16(solved);
		break;
	default:
		displayColoredText(menu->filters_warn_label, "Wrong detection of sudoku size", "red");
		break;
	}
	char dirname[30];
	cleanPath(filename, dirname);
	//printf("dirname = %s\n", dirname);
	Image **digits = loadCells(sudoku, dirname);
	for (int i = 0; i < nb_cells; i++)
	{
		Image *cell = digits[i];
		toRGBA(cell);
		for (uc n = 0; n < 4; n++)
		{
			uc *channel = cell->channels[n];
			uc *new_channel = newChannel(384 * 384);
			for (int i = 0; i < 384 * 384; i++)
			{
				new_channel[i] = 0;
			}
			for (int y = 0; y < 256; y++)
			{
				for (int x = 0; x < 256; x++)
				{
					new_channel[(y + 64) * 384 + (x + 64)] = channel[y * 256 + x];
				}
			}
			free(channel);
			cell->channels[n] = new_channel;
		}
		cell->width = 384;
		cell->height = 384;
	}
	for (int j = 0; j < nb_cells; j++)
	{
		for (int i = 0; i < nb_cells; i++)
		{
			if (!sudoku[j][i] && solved[j][i])
			{
				int n = solved[j][i];
				placeDigit(final, digits[n - 1], quad, coords_x[i], coords_y[j]);
			}
		}
	}
	free(coords_x);
	free(coords_y);
	if (menu->solvedImage)
		freeImage(menu->solvedImage);
	menu->solvedImage = copyImage(final);
	if (menu->redimImage)
		freeImage(menu->redimImage);
	autoResize(final, WINDOW_WIDTH * IMAGE_RATIO, WINDOW_HEIGHT * IMAGE_RATIO);
	menu->redimImage = final;
	displayColoredText(menu->filters_warn_label, "Solved", "green");
	freeQuad(quad);
	freeImage(image);
	for (int i = 0; i < nb_cells; i++)
	{
		free(sudoku[i]);
		free(solved[i]);
		freeImage(digits[i]);
	}
	free(sudoku);
	free(solved);
	free(digits);
	return final;
}