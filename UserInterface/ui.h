#pragma once

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <fts.h>
#include <sys/stat.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>

#include "../ImageProcessing/display.h"
#include "../ImageProcessing/filters.h"
#include "../ImageProcessing/hough.h"
#include "../ImageProcessing/saveImage.h"
#include "../ImageProcessing/tools.h"
#include "../ImageProcessing/transformImage.h"
#include "../ImageProcessing/cellExtraction.h"
#include "../ImageProcessing/cellsDetection.h"
#include "../ImageProcessing/centerCell.h"
#include "../ImageProcessing/param.h"

#include "../NeuralNetwork/Network.h"

#include "../Solver/solver.h"
#include "../Solver/solver16.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

typedef struct
{
	GtkBuilder *builder;
	GtkWindow *window;
	GtkFixed *fixed1;
	GtkBox *box;
	GtkWidget *back_to_menu;
	GtkWidget *file_select_grid;

	GtkWidget *sudoku_image;
	Point *imageOrigin;

	GtkButton *resetFilters_button;
	GtkButton *grayscale_button;
	GtkButton *gaussian_button;
	GtkButton *sobel_button;

	GtkButton *autoDetect_button;
	GtkLabel *manuDetect_label;
	GtkButton *rotate_left_button;
	GtkButton *rotate_right_button;
	GtkEventBox **crop_corners;

	GtkButton *solve_button;
	GtkButton *save_button;

	GtkWidget *filters_grid;

	Image *originImage;
	Image *redimImage;

	GtkLabel *upload_warn_label;
	GtkLabel *filters_warn_label;

} Menu;

typedef enum
{
	FILTERS,
	DETECTION,
	NEURAL_NETWORK,
	SOLVE
} MenuStates;

void uiLaunch();
int rmDir(const char *dir);
int isRegFile(char *path);
void listDir(char *filename);