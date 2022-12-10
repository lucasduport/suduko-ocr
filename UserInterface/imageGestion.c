#include "imageGestion.h"
#include <sys/stat.h>
#include <sys/types.h>

unsigned int cpt = 0;
void newSudokuImage(Menu *menu, char *filename)
{
	Image *image = openImage(filename, 4);
	menu->originImage = image;
	menu->originPath = filename;
	menu->redimImage = copyImage(image);
	autoResize(menu->redimImage, WINDOW_WIDTH * IMAGE_RATIO, WINDOW_HEIGHT * IMAGE_RATIO);
	SudokuImageFromImage(menu, menu->redimImage);
}

void SudokuImageFromImage(Menu *menu, Image *image)
{
	if (menu->sudoku_image != NULL)
		gtk_widget_destroy(menu->sudoku_image);

	char destname[100];
	tmpSaveImage(image, destname);

	menu->sudoku_image = gtk_image_new_from_file(destname);
	gtk_widget_show(menu->sudoku_image);
	gtk_container_add(GTK_CONTAINER(menu->fixed1), menu->sudoku_image);
	gtk_fixed_move(GTK_FIXED(menu->fixed1), menu->sudoku_image,
		(WINDOW_WIDTH - 245 - image->width) / 2, (WINDOW_HEIGHT - 450) / 2);
	menu->imageOrigin->x = (WINDOW_WIDTH - 245 - image->width) / 2;
	menu->imageOrigin->y = (WINDOW_HEIGHT - 450) / 2;
}

void destroySudokuImage(Menu *menu)
{
	if (menu->originImage != NULL)
	{
		freeImage(menu->originImage);
		menu->originImage = NULL;
	}
	if (menu->sudoku_image != NULL)
	{
		gtk_widget_destroy(menu->sudoku_image);
		menu->sudoku_image = NULL;
	}
	if (menu->originPath != NULL)
	{
		g_free(menu->originPath);
		menu->originPath = NULL;
	}
	if (menu->redimImage != NULL)
	{
		freeImage(menu->redimImage);
		menu->redimImage = NULL;
	}
	if (menu->solvedImage != NULL)
	{
		freeImage(menu->solvedImage);
		menu->solvedImage = NULL;
	}
	return;
}

Image *actualImage(Menu *menu)
{
	Image *toPrint = copyImage(menu->redimImage);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->gaussian_button)))
		gaussianBlur(toPrint);
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(menu->sobel_button)))
		sobelFilter(toPrint);
	return toPrint;
}

void refreshImage(GtkWidget *widget, gpointer data)
{
	(void)widget;
	// avoid warning about unused parameter
	Menu *menu = (Menu *)data;
	Image *toPrint = actualImage(menu);
	SudokuImageFromImage(menu, toPrint);
	freeImage(toPrint);
}

void tmpSaveImage(Image *image, char *destname)
{
	SDL_Surface *surface = imageToSurface(image);
	struct stat st_ = {0};
	if (stat("tmpImg/", &st_) == -1)
		mkdir("tmpImg/", 448);
	sprintf(destname, "tmpImg/%d.png", cpt++);
	if (IMG_SavePNG(surface, destname) != 0)
		errx(1, "Error while saving temp image");
	SDL_FreeSurface(surface);
	return;
}

void loadImage(Menu *menu, char *filename)
{
	if (isLoadableImage(filename) == FALSE)
	{
		displayColoredText(menu->upload_warn_label, "This format is not loadable", "red");
		return;
	}
	gtk_window_set_title(menu->window, filename);
	newSudokuImage(menu, filename);
	GtkWidget *to_hide[] = {menu->file_select_grid, NULL};
	GtkWidget *to_show[] = {menu->filters_grid, menu->back_to_menu, NULL};
	widgetCleanup(to_hide, to_show);
}

gboolean isLoadableImage(char *path)
{
	SDL_Surface *test = IMG_Load(path);
	if (test == NULL)
		return FALSE;
	SDL_FreeSurface(test);
	return TRUE;
}

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

Image* solveForUI(Menu *menu)
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
	int **sudoku = (int **)malloc(9 * sizeof(int *));
	for (int i = 0; i < 9; i++)
	{
		sudoku[i] = (int *)malloc(9 * sizeof(int));
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
	menu->solvedImage = copyImage(final);
	autoResize(final, WINDOW_WIDTH * IMAGE_RATIO, WINDOW_HEIGHT * IMAGE_RATIO);
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