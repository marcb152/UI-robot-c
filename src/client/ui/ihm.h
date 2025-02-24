//
// Created by marc on 28/01/25.
//

#ifndef IHM_H
#define IHM_H

#include <gtk/gtk.h>

/**
 * @brief Fonction principale pour l'interface graphique GUI
 * 
 * @param argc Nombre d'arguments
 * @param argv Tableau d'arguments
 */
void gtk_draw(int argc, char** argv);

/**
 * @brief Fonction de callback pour les boutons
 * 
 * @param widget Widget qui a émis le signal
 * @param data Données associées au signal
 */
static void on_button_clicked(GtkWidget *widget, gpointer data);

/**
 * @brief Fonction de callback pour la fermeture de la fenêtre
 * 
 * @param widget Widget qui a émis le signal
 * @param data Données associées au signal
 */
void destroy(GtkWidget* widget, gpointer data);

/**
 * @brief Fonction pour allouer dynamiquement la mémoire pour path
 * 
 * @param size Taille de l'array
 */
void allocate_path(int size);

#endif //IHM_H
