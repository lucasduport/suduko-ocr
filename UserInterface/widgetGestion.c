#include "widgetGestion.h"

void widgetCleanup(GtkWidget **to_hide, GtkWidget **to_show)
{
	widgetHider(to_hide);
	widgetDisplayer(to_show);
	return;
}

void widgetDisplayer(GtkWidget **widgets)
{
	for (size_t i = 0; widgets[i] != NULL; i++)
	{
		gtk_widget_show(GTK_WIDGET(widgets[i]));
	}
	return;
}

void changeSensivityWidgets(GtkWidget **widget, int true)
{
	if (true)
	{
		for (int i = 0; widget[i] != NULL; i++)
		{
			gtk_widget_set_sensitive(widget[i], TRUE);
		}
	}
	else
	{
		for (int i = 0; widget[i] != NULL; i++)
		{
			gtk_widget_set_sensitive(widget[i], FALSE);
		}
	}
}

void widgetHider(GtkWidget **widgets)
{
	for (size_t i = 0; widgets[i] != NULL; i++)
		gtk_widget_hide(GTK_WIDGET(widgets[i]));
	return;
}

gboolean waitForHideWarning(gpointer data)
{
	GtkLabel *label = (GtkLabel *)data;
	gtk_widget_hide(GTK_WIDGET(label));
	return FALSE;
}

void displayWarning(GtkLabel *label, char *message)
{
	gtk_label_set_text(label, message);
	gtk_widget_show(GTK_WIDGET(label));
	g_timeout_add_seconds(1, waitForHideWarning, label);
	return;
}