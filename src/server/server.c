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
#include "robot_app/copilot.h"

#define MAX_PENDING_CONNECTIONS (5)

static int socket_id = 0;
static int client_id = 0;

void communication_avec_client(void)
{
    message_t data = {};

    int return_value = read(client_id, &data, sizeof(data));

    switch (data.command)
    {
    	case COPILOT_INIT:
          	step_t* path = calloc(data.number, sizeof(step_t));  // Allouer dynamiquement la mémoire
        	copilot_init(path, data.number);  // Initialiser le copilot avec le path et le nombre d'étapes
			break;
		case COPILOT_MOVE:
            copilot_move();
            break;
        case COPILOT_IS_PATH_COMPLETE:
            copilot_is_path_complete();
            break;
        case COPILOT_ADD_STEP:
            copilot_add_step(data.number, &data.step);
            break;
        case COPILOT_GET_STEP:
            copilot_get_step(data.number);
            break;
        case COPILOT_RM_STEP:
            copilot_rm_step(data.number);
            break;
        case COPILOT_DISPOSE:
            copilot_dispose();
            break;
        case COPILOT_SAVE:
            copilot_save("path.txt");
            break;
        case COPILOT_LOAD:
            copilot_load("path.txt");
            break;
        case COPILOT_STOP:
            copilot_stop();
            stop_and_disconnect();
            break;
    }
    fprintf(stdout, "Command received: %s\n", command_names[data.command]);
    fprintf(stdout, "Data received: %d\n", data.number);
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

    // Accept clients
    client_id = accept(socket_id, NULL, NULL);
    if (client_id < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    // Handle communication in a new function
    communication_avec_client();
}
