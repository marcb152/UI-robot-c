//
// Created by marc on 20/02/25.
//

#include "server.h"

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

#define MAX_PENDING_CONNECTIONS (5)

static int socket_id = 0;

void communication_avec_client(int socket)
{
    message_t data = {};

    while (1)
    {
        int return_value = read(socket, &data, sizeof(data));
        fprintf(stdout, "Command received: %d\n", data.command);
        fprintf(stdout, "Data received: %d\n", data.number);
    }

    /* la connexion est normalement déjà fermée par le client, mais la prudence ne coûte rien */
//    close (socket_id);

//    exit (0);
}

int start_and_connect(void)
{
    struct sockaddr_in mon_adresse;
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    mon_adresse.sin_family = AF_INET;
    mon_adresse.sin_port = htons(PORT_DU_SERVEUR);
    mon_adresse.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind with error handling
    if (bind(socket_id, (struct sockaddr *)&mon_adresse, sizeof(mon_adresse)) < 0)
    {
        perror("Bind failed");
        close(socket_id);
        exit(EXIT_FAILURE);
    }

    // Listen with error handling
    if (listen(socket_id, MAX_PENDING_CONNECTIONS) < 0)
    {
        perror("Listen failed");
        close(socket_id);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT_DU_SERVEUR);

    // Accept loop for multiple clients
    while (1)
    {
        int client_socket = accept(socket_id, NULL, NULL);
        if (client_socket < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("Client connected!\n");

        // Handle communication in a new function
        communication_avec_client(client_socket);
    }
}
