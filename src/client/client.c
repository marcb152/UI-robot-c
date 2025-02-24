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

static int socket_id = 0;

message_t * read_return_value(void)
{
    message_t * return_value = malloc(sizeof(message_t));
    if (read(socket_id, return_value, sizeof(message_t)) < 0)
    {
        perror("Error reading return value");
        return NULL;
    }
    return return_value;
}

int start_and_connect(void)
{
    struct sockaddr_in adresse_du_serveur;
    message_t donnees;

    socket_id = socket (AF_INET, SOCK_STREAM, 0);

    adresse_du_serveur.sin_family = AF_INET;
    adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);
    /* adresse IP (ou nom de domaine) du serveur auquel se connecter */
    adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname ("127.0.0.1")->h_addr_list[0]);
    /* On demande la connexion auprès du serveur (même principe que bind, mais côté client) */

    int return_value = connect (socket_id, (struct sockaddr *)&adresse_du_serveur, sizeof (adresse_du_serveur));

    // TODO: Return error message if connection fails
    return 0;
}

int stop_and_disconnect(void)
{
    close (socket_id);

    socket_id = 0;

    return 0;
}

int send_data(message_t data)
{
    if (socket_id)
    {
        int return_code = write(socket_id, &data, sizeof(data));
        if (return_code < 0)
        {
            fprintf(stderr, "Error sending data: %s\n", strerror(errno));
            return -1;
        }
    }
    return 0;
}

void socket_copilot_init(int size)
{
    message_t data = {.command = COPILOT_INIT, .number = size};
    send_data(data);
}

void socket_copilot_begin_move(void)
{
    message_t data = {.command = COPILOT_MOVE};
    send_data(data);
}

int socket_copilot_is_path_complete(void)
{
    message_t data = {.command = COPILOT_IS_PATH_COMPLETE};
    send_data(data);
    return read_return_value()->number;
}

void socket_copilot_add_step(int index, step_t *step)
{
    message_t data = {.command = COPILOT_ADD_STEP, .number = index, .step = *step};
    send_data(data);
}

step_t * socket_copilot_get_step(int i)
{
    message_t data = {.command = COPILOT_GET_STEP, .number = i};
    send_data(data);
    return &read_return_value()->step;
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
    return read_return_value()->number;
}

int socket_copilot_load(char *filename)
{
    message_t data = {.command = COPILOT_LOAD};
    send_data(data);
    return read_return_value()->number;
}

void socket_copilot_stop(void)
{
    message_t data = {.command = COPILOT_STOP};
    send_data(data);
}
