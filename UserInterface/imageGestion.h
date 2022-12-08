#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include "interactions.h"
#include "ui.h"
#include "widgetGestion.h"

#define IMAGE_RATIO 0.6

void newSudokuImage(Menu *menu, char *filename);

void SudokuImageFromImage(Menu *menu, Image *image);

void destroySudokuImage(Menu *menu);

Image *actualImage(Menu *menu);

void refreshImage(GtkWidget *widget, gpointer data);

void tmpSaveImage(Image *image, char *destname);

void loadImage(Menu *menu, char *filename);

gboolean isLoadableImage(char *path);