//
// Created by marc on 28/01/25.
//

#ifndef IHM_H
#define IHM_H

#include <gtk/gtk.h>

int gtk_draw(int argc, char** argv);

static void on_button_clicked(GtkWidget *widget, gpointer data);

static void on_combo_changed(GtkWidget *widget, gpointer data);

static void on_value_changed(GtkWidget *widget, gpointer data);


#endif //IHM_H
