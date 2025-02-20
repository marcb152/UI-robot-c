//
// Created by marc on 20/02/25.
//

#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "../common/socket_common.h"

int start_and_connect(void)
{
    int un_socket;
    struct sockaddr_in adresse_du_serveur;
    message_t donnees;

    un_socket = socket (AF_INET, SOCK_STREAM, 0);

    adresse_du_serveur.sin_family = AF_INET;
    adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);
    /* adresse IP (ou nom de domaine) du serveur auquel se connecter */
    adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname ("127.0.0.1")->h_addr_list[0]);
    /* On demande la connexion auprès du serveur (même principe que bind, mais côté client) */

    connect (un_socket, (struct sockaddr *)&adresse_du_serveur, sizeof (adresse_du_serveur));

    read (un_socket, &donnees, sizeof (donnees));

//    donnees.age_capitaine = ntohl (donnees.age_capitaine);

//    printf ("Message reçu : %s\n", donnees.message);
//    printf ("Age du capitaine : %d\n", donnees.age_capitaine);

    close (un_socket);

    // TODO: Return error message if connection fails
    return 0;
}

int send_data(message_t data)
{

}

void socket_copilot_init(step_t *path, int size)
{
    message_t data = {.command = COPILOT_INIT, .number = size};
    send_data(data);
    for (int i = 0; i < size; ++i)
    {
        socket_copilot_add_step(i, &path[i]);
    }
}

void socket_copilot_move(void)
{
    message_t data = {.command = COPILOT_MOVE};
    send_data(data);
}

int socket_copilot_is_path_complete(void)
{
    message_t data = {.command = COPILOT_IS_PATH_COMPLETE};
    send_data(data);
    // TODO: Retrieve data received
}

void socket_copilot_add_step(int index, step_t *step)
{
    message_t data = {.command = COPILOT_ADD_STEP, .step = *step};
    send_data(data);
}

step_t * socket_copilot_get_step(int i)
{
    message_t data = {.command = COPILOT_GET_STEP, .number = i};
    send_data(data);
    // TODO: Retrieve data received
}

void socket_copilot_rm_step(int index)
{
    message_t data = {.command = COPILOT_RM_STEP, .number = index};
    send_data(data);
}

void socket_copilot_dispose(void)
{
    message_t data = {.command = COPILOT_DISPOSE};
    send_data(data);
}

int socket_copilot_save(char *filename)
{
    message_t data = {.command = COPILOT_SAVE};
    send_data(data);
}

int socket_copilot_load(char *filename)
{
    message_t data = {.command = COPILOT_LOAD};
    send_data(data);
    // TODO: Retrieve data received
}

void socket_copilot_stop(void)
{
    message_t data = {.command = COPILOT_STOP};
    send_data(data);
}
