#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "commun.h"

#define MAX_PENDING_CONNECTIONS (5)

void communication_avec_client (int socket, int age_capitaine)
{
    DesDonnees ma_donnee;
    int quantite_envoyee;

    strcpy (ma_donnee.message, "bonjour");
    ma_donnee.age_capitaine = htonl (age_capitaine);

    /* on envoie une donnee jusqu'à ce que le client ferme la connexion */
    quantite_envoyee = write (socket, &ma_donnee, sizeof (ma_donnee));

    /* la connexion est normalement déjà fermée par le client, mais la prudence ne coûte rien */
    close (socket);

    exit (0);	
}

int main (void)
{
    int socket_ecoute;
    int socket_donnees;
    int age_capitaine_courant = 25;
    struct sockaddr_in mon_adresse;

    socket_ecoute = socket(AF_INET, SOCK_STREAM,0);

    mon_adresse.sin_family = AF_INET;
    mon_adresse.sin_port = PORT_DU_SERVEUR;
    mon_adresse.sin_addr.s_addr = htonl (INADDR_ANY); /* On s'attache a toutes les interfaces */
    /* On attache le socket a l'adresse indiquee */
    bind (socket_ecoute, (struct sockaddr *)&mon_adresse, sizeof (mon_adresse));
    
    listen(socket_ecoute, MAX_PENDING_CONNECTIONS);

    socket_ecoute = accept(socket_ecoute,NULL,NULL);

    communication_avec_client(socket_ecoute,25);

}
