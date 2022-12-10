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

void changeSensivityWidgets(GtkWidget **widget, int sensitive)
{
	if (sensitive)
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

void displayColoredText(GtkLabel *label, char *message, char *color)
{
	if (strcmp(color, "red") == 0)
		gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &(GdkRGBA) { 1, 0, 0 });
	else if (strcmp(color, "green") == 0)
		gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &(GdkRGBA) { 0, 1, 0 });
	else if (strcmp(color, "blue") == 0)
		gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &(GdkRGBA) { 0, 0, 1 });
	else if (strcmp(color, "black") == 0)
		gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &(GdkRGBA) { 0, 0, 0 });
	else if (strcmp(color, "white") == 0)
		gtk_widget_override_color(label, GTK_STATE_FLAG_NORMAL, &(GdkRGBA) { 1, 1, 1 });
	gtk_label_set_text(label, message);
	char *markup = g_markup_printf_escaped("<span foreground='%s'><span font_desc='Tlwg Typo Bold 12'>%s</span></span>", color, message);
	gtk_label_set_markup(GTK_LABEL(label), markup);
	gtk_widget_show(GTK_WIDGET(label));
	g_timeout_add_seconds(2, waitForHideWarning, label);
	free(markup);
	return;
}