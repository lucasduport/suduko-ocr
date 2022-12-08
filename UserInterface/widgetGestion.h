#pragma once

#include <gtk/gtk.h>
#include "ui.h"

void widgetDisplayer(GtkWidget **widgets);
void widgetHider(GtkWidget **widgets);
void widgetCleanup(GtkWidget **to_hide, GtkWidget **to_show);
void displayWarning(GtkLabel *label, char *message);
void changeSensivityWidgets(GtkWidget **widget, int true);
