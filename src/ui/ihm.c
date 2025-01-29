#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked: %s\n", (char *)data);
}

static void on_value_changed(GtkWidget *widget, gpointer data) {
    gint value = gtk_range_get_value(GTK_RANGE(widget));
    g_print("slider1 value: %d\n", value);
}

static void on_combo_changed(GtkWidget *widget, gpointer data) {
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    g_print("Selected option: %s\n", text);
    g_free(text);
}

int gtk_draw(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox, *grid1, *hbox2, *hbox3;
    GtkWidget *button_save, *button_add, *slider1, *slider2;
    GtkWidget *combo_box, *text_entry;
    GtkWidget *separator1, *separator2;

    gtk_init(&argc, &argv);

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Interface avec options");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 800);
    gtk_container_set_border_width(GTK_CONTAINER(window), 25);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la boîte verticale principale
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Création de la grille pour organiser les widgets dans la première boîte
    grid1 = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid1), 10);  // Espacement entre les lignes
    gtk_grid_set_column_spacing(GTK_GRID(grid1), 10);  // Espacement entre les colonnes

    // Création des boutons SAVE et ADD
    button_save = gtk_button_new_with_label("SAVE");
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_button_clicked), "SAVE");
    
    button_add = gtk_button_new_with_label("ADD");
    g_signal_connect(button_add, "clicked", G_CALLBACK(on_button_clicked), "ADD");

    // Création du slider1 allant de 0 à 100
    slider1 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_range_set_value(GTK_RANGE(slider1), 50);
    g_signal_connect(slider1, "value-changed", G_CALLBACK(on_value_changed), NULL);

    // Création du slider2 pour l'angle
    slider2 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -360, 360, 1);
    gtk_range_set_value(GTK_RANGE(slider2), 0);
    g_signal_connect(slider2, "value-changed", G_CALLBACK(on_value_changed), NULL);
    
    // Création du menu déroulant avec FORWARD et ROTATION
    combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "FORWARD");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "ROTATION");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);  // "FORWARD" par défaut
    g_signal_connect(combo_box, "changed", G_CALLBACK(on_combo_changed), NULL);

    // Création de la zone de texte pour entrer du texte
    text_entry = gtk_entry_new();

    // Ajout des widgets dans la grille (placement dans les cellules)
    gtk_grid_attach(GTK_GRID(grid1), button_save, 0, 0, 1, 1);  // Colonne, Ligne, Size
    gtk_grid_attach(GTK_GRID(grid1), button_add, 0, 1, 1, 2);   // Ligne 0, Colonne 1
    gtk_grid_attach(GTK_GRID(grid1), slider1, 3, 1, 8, 1);       // Ligne 1, Colonne 0-1
    gtk_grid_attach(GTK_GRID(grid1), slider2, 1, 2, 8, 1);       // Ligne 1, Colonne 0-1
    gtk_grid_attach(GTK_GRID(grid1), combo_box, 1, 1, 2, 1);    // Ligne 2, Colonne 0-1
    gtk_grid_attach(GTK_GRID(grid1), text_entry, 1, 0, 10, 1);   // Ligne 3, Colonne 0-1

    // Création des boîtes horizontales pour les autres boutons
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Création des boutons pour les boîtes 2 et 3
    GtkWidget *button2 = gtk_button_new_with_label("Bouton 2");
    GtkWidget *button3 = gtk_button_new_with_label("Bouton 3");
    
    // Ajout des boutons dans les boîtes 2 et 3
    gtk_box_pack_start(GTK_BOX(hbox2), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox3), button3, TRUE, TRUE, 0);

    // Création des séparateurs
    separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    // Ajout des widgets dans la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), grid1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), separator1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), separator2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);

    // Make widgets inside the window expand to fill available space
    gtk_widget_set_vexpand(window, TRUE);

    // Affichage de la fenêtre
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
