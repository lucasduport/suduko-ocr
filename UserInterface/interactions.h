#pragma once

#include "imageGestion.h"
#include "ui.h"
#include "widgetGestion.h"

void on_upload_button_clicked(GtkWidget *widget, gpointer data);

void on_upload_entry_activate(GtkWidget *widget, gpointer data);

void on_back_to_menu_button_clicked(GtkWidget *widget, gpointer data);

void on_save_clicked(GtkWidget *widget, gpointer data);

void resetFilters(Menu *menu);

void on_resetFilters_clicked(GtkWidget *widget, gpointer data);

void on_autoDetect_clicked(GtkWidget *widget, gpointer data);

void on_solve_clicked(GtkWidget *widget, gpointer data);

void on_grayscale_toggled(GtkWidget *widget, gpointer data);

void on_crop_corners_move(GtkWidget *widget, GdkEvent *event, gpointer data);

void on_rotate_left_clicked(GtkWidget *widget, gpointer data);

void on_rotate_right_clicked(GtkWidget *widget, gpointer data);

void on_manuDetect_clicked(GtkWidget *widget, gpointer data);

void leave_manual_crop(Menu *menu);

void upload_drag_data_received(GtkWidget *widget, GdkDragContext *context,
	gint x, gint y, GtkSelectionData *data, guint info, guint time,
	gpointer userdata);

void entry_drag_data_received(GtkWidget *widget, GdkDragContext *context,
	gint x, gint y, GtkSelectionData *data, guint info, guint time,
	gpointer userdata);

void open_folder_selector(GtkWidget *widget, gpointer data);

//void on_window_resize( GtkWidget* widget, GdkEventConfigure event, gpointer user_data);