//
// Created by marc on 28/01/25.
//

#ifndef IHM_H
#define IHM_H

#include <gtk/gtk.h>

int gtk_draw(int argc, char** argv);

void callback( GtkWidget *widget, gpointer data );

gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data);

void on_button_clicked(GtkWidget *widget, gpointer data);

void on_combo_changed(GtkWidget *widget, gpointer data);

void on_value_changed(GtkWidget *widget, gpointer data);


#endif //IHM_H
