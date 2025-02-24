#include "ihm.h"

#include "../../common/pilot_common.h"
#include "../client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int step_index = 0;
static int return_value = 0;

GtkWidget *window;
GtkWidget *vbox, *grid1, *hbox2, *hbox3;
GtkWidget *button_save, *button_load, *button_add, *button_start, *button_quit, *button_delete;
GtkWidget *slider_speed, *slider_angle, *slider_distance;
GtkWidget *lbl_slider_speed, *lbl_slider_angle, *lbl_slider_dst;
GtkWidget *combo_box, *text_entry, *scrollable_window, *listbox, *row, *label;
GtkWidget *separator1, *separator2;

void add_line(int index);

void allocate_path(int size)
{
    socket_copilot_init(size);  // Initialiser le copilot avec le path et le nombre d'étapes
}

void destroy (GtkWidget* widget, gpointer data)
{
    if(widget == button_quit)
    {
        return_value = 1;
    }
    gtk_widget_destroy(window);
    gtk_main_quit();
}

static void on_button_clicked(GtkWidget *widget, gpointer data) 
{
    g_print("Button clicked: %s\n", (char *)data);

    if (widget == button_add) 
    {
        // Récupérer les valeurs des sliders au moment de l'ajout
        gint speed_value = gtk_range_get_value(GTK_RANGE(slider_speed));
        gint angle_value = gtk_range_get_value(GTK_RANGE(slider_angle));
        gint distance_value = gtk_range_get_value(GTK_RANGE(slider_distance));
        gint action_value = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));

        step_t step = { {action_value, angle_value, distance_value}, speed_value };

        // If the user wants to add a step at the end of the path that doesn't exist yet
        socket_copilot_add_step(step_index, &step);

        // Format du texte : "Step 1 | direction | angle | distance | vitesse"
        add_line(step_index);

        step_index++;

        // Réafficher la fenêtre pour actualiser la listbox
        gtk_widget_show_all(window);
    } 
    else if (widget == button_save) 
    {
        // Sauvegarder avec le bon nom de fichier
        socket_copilot_save("path.txt");
    }
    else if (widget == button_load)
    {
        // Chargement avec le bon nom de fichier
        const int temp = socket_copilot_load("path.txt");
        if (temp != -1)
        {
            step_index = temp;

            // Actualiser la listbox avec les éléments chargés
            for (int i = 0 ; i < temp ; i++)
            {
                add_line(i);
            }
        }
        gtk_widget_show_all(window);
    }
}

void delete_step(GtkWidget *widget, gpointer data)
{
    void* selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox));
    if (!selected_row)
    {
        return;
    }
    int index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(selected_row));
    socket_copilot_rm_step(index);
    gtk_container_remove(GTK_CONTAINER(listbox), selected_row);
    fprintf(stdout,"%d", index);
    step_index--;
}

void add_line(int index)
{
    char text[200];
    step_t * step = socket_copilot_get_step(index);
    const char *direction = (step->move.action == FORWARD) ? "FORWARD" : "ROTATION";
    
    snprintf(text, sizeof(text), "Step %d | %s\n\t | Angle: %d\n\t | Distance: %d\n\t | Speed: %d", index + 1, direction, step->move.angle, step->move.distance, step->speed);

    label = gtk_label_new(text);
    row = gtk_list_box_row_new();
    gtk_container_add(GTK_CONTAINER(row), label);
    gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
}

// Fonction principale pour l'interface graphique GUI
int gtk_draw(int argc, char *argv[])
{
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Allouer dynamiquement la mémoire pour path
    allocate_path(1);

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Robot Interface");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 25);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

    // Création de la boîte verticale principale
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Création de la grille pour organiser les widgets dans la première boîte
    grid1 = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid1), 10);  // Espacement entre les lignes
    gtk_grid_set_column_spacing(GTK_GRID(grid1), 10);  // Espacement entre les colonnes

    // Création des boutons
    button_save = gtk_button_new_with_label("SAVE");
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_button_clicked), "SAVE");

    button_load = gtk_button_new_with_label("LOAD");
    g_signal_connect(button_load, "clicked", G_CALLBACK(on_button_clicked), "LOAD");

    button_add = gtk_button_new_with_label("ADD");
    g_signal_connect(button_add, "clicked", G_CALLBACK(on_button_clicked), "ADD");

    button_quit = gtk_button_new_with_label("QUIT");
    g_signal_connect(button_quit, "clicked", G_CALLBACK(destroy), "QUIT");

    button_start = gtk_button_new_with_label("START");
    g_signal_connect(button_start, "clicked", G_CALLBACK(destroy), "START");

    button_delete = gtk_button_new_with_label("DEL");
    g_signal_connect(button_delete, "clicked", G_CALLBACK(delete_step), "DEL");


    // Création du slider1 allant de 0 à 100
    slider_speed = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_range_set_value(GTK_RANGE(slider_speed), 50);
    lbl_slider_speed = gtk_label_new("Speed");

    // Création du slider2 pour l'angle
    slider_angle = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -360, 360, 1);
    gtk_range_set_value(GTK_RANGE(slider_angle), 0);
    lbl_slider_angle = gtk_label_new("Angle");

    // Création du slider3 pour la distance
    slider_distance = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1000, 1);
    gtk_range_set_value(GTK_RANGE(slider_distance), 0);
    lbl_slider_dst = gtk_label_new("Distance");

    // Création du menu déroulant avec FORWARD et ROTATION
    combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "FORWARD");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "ROTATION");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);  // "FORWARD" par défaut

    // Création de la zone de texte pour entrer du texte
    text_entry = gtk_entry_new();       // Actuellement inutile car on save le path dans un fichier text déjà nommé.

    // Création de la liste scrollable
    scrollable_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrollable_window), listbox);

    // Ne plus utiliser la boucle 'for' qui remplissait la listbox initialement

    // Réduire l'espace entre les lignes et les colonnes dans la grille
    gtk_grid_set_row_spacing(GTK_GRID(grid1), 3);  // Espacement entre les lignes
    gtk_grid_set_column_spacing(GTK_GRID(grid1), 3);  // Espacement entre les colonnes

    // Réduire la largeur du slider
    gtk_widget_set_size_request(slider_speed, 200, -1);  // 200 pixels de large, hauteur auto
    gtk_widget_set_size_request(slider_angle, 200, -1);
    gtk_widget_set_size_request(slider_distance, 200, -1);

    // Réduire la marge autour de la fenêtre
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Ajout des widgets dans la grille (placement dans les cellules)
    gtk_grid_attach(GTK_GRID(grid1), button_save, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), button_load, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), button_add, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), button_delete, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), button_start, 0, 5, 20, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider_speed, 3, 1, 18, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider_angle, 1, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider_distance, 3, 2, 18, 1);
    gtk_grid_attach(GTK_GRID(grid1), combo_box, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid1), text_entry, 2, 0, 20, 1);
    gtk_grid_attach(GTK_GRID(grid1), lbl_slider_speed, 4, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), lbl_slider_angle, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), lbl_slider_dst, 4, 2, 1, 1);

    // Création des boîtes horizontales pour les autres boutons
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    
    // Ajout du bouton QUIT
    gtk_box_pack_start(GTK_BOX(hbox3), button_quit, TRUE, TRUE, 0);

    // Création des séparateurs
    separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    // Ajout des widgets dans la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), grid1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), separator1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scrollable_window, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), separator2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, TRUE, 0);

    // Make widgets inside the window expand to fill available space
    gtk_widget_set_vexpand(window, TRUE);

    // Affichage de la fenêtre
    gtk_widget_show_all(window);

    // Lancement de la boucle principale
    gtk_main();

    return return_value;
}
