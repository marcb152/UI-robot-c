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

int main (void)
{
  int un_socket;
  struct sockaddr_in adresse_du_serveur;
  DesDonnees donnees;
  
  un_socket = socket (AF_INET, SOCK_STREAM, 0);

  adresse_du_serveur.sin_family = AF_INET;
  adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);
  /* adresse IP (ou nom de domaine) du serveur auquel se connecter */
  adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname ("127.0.0.1")->h_addr_list[0]);
  /* On demande la connexion auprès du serveur (même principe que bind, mais côté client) */

  connect (un_socket, (struct sockaddr *)&adresse_du_serveur, sizeof (adresse_du_serveur));
  
  read (un_socket, &donnees, sizeof (donnees));

  donnees.age_capitaine = ntohl (donnees.age_capitaine);

  printf ("Message reçu : %s\n", donnees.message);
  printf ("Age du capitaine : %d\n", donnees.age_capitaine);

  close (un_socket);
}
