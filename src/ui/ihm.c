#include "ihm.h"
#include "../robot_app/copilot.h"
#include "../robot_app/pilot.h"
#include "../robot_app/robot.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int size = 5;
int step_index = 0;

GtkWidget *window;
GtkWidget *vbox, *grid1, *hbox2, *hbox3;
GtkWidget *button_save, *button_add, *button_start, *button_quit;
GtkWidget *slider1, *slider2, *slider3;
GtkWidget *slider1_label, *slider2_label, *slider3_label;
GtkWidget *combo_box, *text_entry, *scrollable_window, *listbox, *row, *label;
GtkWidget *separator1, *separator2;

move_t *path;  // Déclaration sans initialisation

void allocate_path(int size) {
    path = calloc(size, sizeof(move_t));  // Allouer dynamiquement la mémoire
    if (path != NULL) {
        copilot_init(path, size);  // Initialiser le copilot avec le path et le nombre d'étapes
    } else {
        fprintf(stderr, "Memory allocation for path failed\n");
    }
}

static void on_button_clicked(GtkWidget *widget, gpointer data) 
{
    g_print("Button clicked: %s\n", (char *)data);

    if (widget == button_add) 
    {
        // Récupérer les valeurs des sliders au moment de l'ajout
        gint speed_value = gtk_range_get_value(GTK_RANGE(slider1));
        gint angle_value = gtk_range_get_value(GTK_RANGE(slider2));
        gint distance_value = gtk_range_get_value(GTK_RANGE(slider3));

        // Enregistrer ces valeurs dans l'étape actuelle
        path[step_index].angle = angle_value;
        path[step_index].distance = distance_value;

        // Format du texte : "Step 1 | direction | angle | distance | vitesse"
        char text[200];
        const char *direction = (path[step_index].action == FORWARD) ? "FORWARD" : "ROTATION";
        
        // En fonction de l'action, on affiche l'angle ou la distance
        int angle_or_distance = (path[step_index].action == FORWARD) ? path[step_index].distance : path[step_index].angle;
        
        // Ajouter le texte formaté à la listbox
        snprintf(text, sizeof(text), "Step %d | %s | %d | %d", 
                 step_index + 1, direction, path[step_index].angle, 
                 path[step_index].distance);

        label = gtk_label_new(text);
        row = gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);  // Ajouter à la fin de la liste

        printf("Contenu de path :\n");

        step_index++;  // Incrémenter l'index

        // Réafficher la fenêtre pour actualiser la listbox
        gtk_widget_show_all(window);

        for (int i = 0; i < step_index; i++)  // Parcours de toutes les étapes ajoutées
        {
            const char *direction = (path[i].action == FORWARD) ? "FORWARD" : "ROTATION";
            
            // En fonction de l'action, on affiche l'angle ou la distance
            int angle_or_distance = (path[i].action == FORWARD) ? path[i].distance : path[i].angle;
            
            // Affichage formaté
            printf("Step %d | %s | Angle: %d | Distance: %d\n",
                i + 1, direction, path[i].angle, path[i].distance);
        }
    } 
    else if (widget == button_save) 
    {
        // Sauvegarder avec le bon nom de fichier
        copilot_save("path.txt");
    }
    else if (widget == button_start || step_index == size) 
    {
        copilot_move();
    }
}

static void on_value_changed(GtkWidget *widget, gpointer data) 
{
    gint value = gtk_range_get_value(GTK_RANGE(widget));
    g_print("slider1 value: %d\n", value);

    if (widget == slider1) 
    {
        speed_pct_t speed = (speed_pct_t)value;
        robot_set_speed(speed, speed);
    } 
    else if (widget == slider2) 
    {
        path[step_index].angle = (int)value;
    } 
    else if (widget == slider3) 
    {
        path[step_index].distance = (int)value;
    }
}

static void on_combo_changed(GtkWidget *widget, gpointer data) 
{
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    g_print("Selected option: %s\n", text);

    // Comparaison correcte des chaînes
    if (strcmp(text, "FORWARD") == 0) 
    {
        path[step_index].action = FORWARD;
    } 
    else if (strcmp(text, "ROTATION") == 0) 
    {
        path[step_index].action = ROTATION;
    }

    g_free(text);  // Libérer la mémoire allouée pour 'text'
}

int gtk_draw(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    // Allouer dynamiquement la mémoire pour path
    allocate_path(size);

    // Création de la fenêtre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Robot Interface");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 25);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

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
    
    button_add = gtk_button_new_with_label("ADD");
    g_signal_connect(button_add, "clicked", G_CALLBACK(on_button_clicked), "ADD");

    button_quit = gtk_button_new_with_label("QUIT");
    g_signal_connect(button_quit, "clicked", G_CALLBACK(gtk_main_quit), "QUIT");

    button_start = gtk_button_new_with_label("START");
    g_signal_connect(button_quit, "clicked", G_CALLBACK(copilot_move), "START");

    // Création du slider1 allant de 0 à 100
    slider1 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_range_set_value(GTK_RANGE(slider1), 50);
    g_signal_connect(slider1, "value-changed", G_CALLBACK(on_value_changed), NULL);
    slider1_label = gtk_label_new("Speed");

    // Création du slider2 pour l'angle
    slider2 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, -360, 360, 1);
    gtk_range_set_value(GTK_RANGE(slider2), 0);
    g_signal_connect(slider2, "value-changed", G_CALLBACK(on_value_changed), NULL);
    slider2_label = gtk_label_new("Angle");

    // Création du slider3 pour la distance
    slider3 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 1000, 1);
    gtk_range_set_value(GTK_RANGE(slider3), 0);
    g_signal_connect(slider3, "value-changed", G_CALLBACK(on_value_changed), NULL);
    slider3_label = gtk_label_new("Distance");

    // Création du menu déroulant avec FORWARD et ROTATION
    combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "FORWARD");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), "ROTATION");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);  // "FORWARD" par défaut
    g_signal_connect(combo_box, "changed", G_CALLBACK(on_combo_changed), NULL);

    // Création de la zone de texte pour entrer du texte
    text_entry = gtk_entry_new();

    // Création de la liste scrollable
    scrollable_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollable_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    listbox = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrollable_window), listbox);

    // Ne plus utiliser la boucle 'for' qui remplissait la listbox initialement

    // Réduire l'espace entre les lignes et les colonnes dans la grille
    gtk_grid_set_row_spacing(GTK_GRID(grid1), 5);  // Espacement entre les lignes
    gtk_grid_set_column_spacing(GTK_GRID(grid1), 5);  // Espacement entre les colonnes

    // Réduire la largeur du slider
    gtk_widget_set_size_request(slider1, 200, -1);  // 200 pixels de large, hauteur auto
    gtk_widget_set_size_request(slider2, 200, -1);
    gtk_widget_set_size_request(slider3, 200, -1);

    // Réduire la hauteur des séparateurs
    gtk_widget_set_size_request(separator1, -1, 1);  // Hauteur réduite
    gtk_widget_set_size_request(separator2, -1, 1);

    // Réduire la marge autour de la fenêtre
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Ajout des widgets dans la grille (placement dans les cellules)
    gtk_grid_attach(GTK_GRID(grid1), button_save, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), button_add, 0, 1, 1, 2);
    gtk_grid_attach(GTK_GRID(grid1), button_start, 0, 5, 20, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider1, 3, 1, 18, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider2, 1, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider3, 3, 2, 18, 1);
    gtk_grid_attach(GTK_GRID(grid1), combo_box, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid1), text_entry, 1, 0, 20, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider1_label, 4, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider2_label, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), slider3_label, 4, 2, 1, 1);

    // Création des boîtes horizontales pour les autres boutons
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    
    // Ajout du bouton QUIT
    gtk_box_pack_start(GTK_BOX(hbox3), button_quit, TRUE, TRUE, 0);

    // Création des séparateurs
    separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    // Ajout des widgets dans la boîte verticale
    gtk_box_pack_start(GTK_BOX(vbox), grid1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), separator1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scrollable_window, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), separator2, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox3, TRUE, TRUE, 0);

    // Make widgets inside the window expand to fill available space
    gtk_widget_set_vexpand(window, TRUE);

    // Affichage de la fenêtre
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
