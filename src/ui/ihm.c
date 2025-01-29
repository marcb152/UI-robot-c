//
// Created by marc on 28/01/25.
//

#include "ihm.h"
#include <gtk/gtk.h>

int gtk_draw(int argc, char** argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_widget_show_all(window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
}