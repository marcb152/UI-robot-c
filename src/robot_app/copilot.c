//
// Created by marc on 23/01/25.
//

#include "copilot.h"
#include "robot.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static step_t * path;
static volatile int size;
static int step_counter = 0;

void copilot_init(step_t * path_ptr, int path_size)
{
    step_counter = 0;
    size = path_size;
    path = path_ptr;
}

void copilot_move(void)
{
    robot_status_t my_status = robot_get_status();
    pilot_start_move(&path[step_counter % size].move);
    fprintf(stdout, "codeurs: g = %d, d = %d\n", my_status.left_encoder,
            my_status.right_encoder);
    fprintf(stdout, "proxy: g = %d, c = %d, d = %d\n", my_status.left_sensor,
            my_status.center_sensor, my_status.right_sensor);
    fprintf(stdout, "batterie: %d %%\n", my_status.battery);
    fprintf(stdout, "STEP: %d\n", step_counter % size);

    /* Encoders polling */
    for (int i = 0; i < ENCODERS_SCAN_NB; i++)
    {
        usleep(DELAY);
        if (pilot_stop_at_target() == MOVE_DONE)
        {
            step_counter++;
            break;
        }
    }
}

int copilot_is_path_complete(void)
{
    return step_counter == size;
}

void copilot_add_step(int index, step_t * step)
{
    if (path)
    {
        if (index >= 0 && index < size)
        {
            // TODO: free the previous step if it exists
            path[index] = *step;
        }
        else
        {
            size++;
            // Nouvelle array avec une plus grande taille
            step_t* temp = calloc(size, sizeof(step_t));
            // Copie des valeurs de l'ancienne array
            for (int i = 0; i < size - 1; i++)
            {
                temp[i] = path[i];
            }
            // Nettoyage de l'ancienne array
            free(path);
            // Mise à jour du pointeur
            path = temp;
            // Ajout du nouveau step
            path[size - 1] = *step;
        }
    }
}

step_t* copilot_get_step(int i)
{
    if (path && i >= 0 && i < size)
    {
        return &path[i];
    }
    return NULL;
}

void copilot_rm_step(int index)
{
    if (path && index >= 0 && index < size)
    {
        // Décalage des valeurs et suppression par écrasement
        for (int i = index; i <= size; ++i)
        {
            path[i] = path[i + 1];
        }
        size--;
        // TODO: free the last step
    }
}

void copilot_dispose(void)
{
    free(path);
    size = 0;
    step_counter = 0;
}

int copilot_save(char *filename)
{
    // Create or overwrite the file
    FILE *file_ptr = fopen(filename, "w");

    // Error handling
    if (!file_ptr)
    {
        return -1;
    }

    /*
     * FORMAT OF THE FILE
     * 1. Number of steps
     * 2. For each step:
     *   - Type of move
     *   - Speed
     *   - Distance or angle (based on type)
     */
    // Loop over the steps and write them to the file
    int i = 0;
    step_t * step;
    if (size > 0)
    {
        fprintf(file_ptr, "%d\n", size);
        for (i = 0; i < size; i++)
        {
            step = copilot_get_step(i);
            // TODO: check if the user didn't messed up the input values
            fprintf(file_ptr, "%d %d %d\n",
              step->move.action,
              step->speed,
              step->move.action == FORWARD ? step->move.distance : step->move.angle);
        }
        fclose(file_ptr);
        return 0;
    }
    fclose(file_ptr);
    return -1;
}

int copilot_load(char *filename)
{
    // Open the file
    FILE *file_ptr = fopen(filename, "r");

    // Error handling
    if (!file_ptr)
    {
        return -1;
    }

    /*
     * FORMAT OF THE FILE
     * 1. Number of steps
     * 2. For each step:
     *   - Type of move
     *   - Speed
     *   - Distance or angle (based on type)
     */
    // Read the number of steps
    int temp;
    if (fscanf(file_ptr, "%d\n", &temp) != 1)
    {
        fclose(file_ptr);
        return -1;
    }

    // Allocate memory for the path
    path = calloc(temp, sizeof(step_t));
    if (!path)
    {
        fclose(file_ptr);
        return -1;
    }

    // Read each step from the file
    for (int i = 0; i < temp; i++)
    {
        int action, speed, value;
        fscanf(file_ptr, "%d %d %d\n", &action, &speed, &value);
        path[i].move.action = action;
        path[i].speed = speed;
        if (action == FORWARD)
        {
            path[i].move.distance = value;
        }
        else
        {
            path[i].move.angle = value;
        }
    }

    // Initialize copilot with the loaded path
    step_counter = 0;
    size = temp;

    // Close the file
    fclose(file_ptr);
    return size;
}
