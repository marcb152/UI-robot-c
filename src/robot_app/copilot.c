//
// Created by marc on 23/01/25.
//

#include "copilot.h"
#include "robot.h"

#include <stdio.h>
#include <unistd.h>

static const move_t * path;
static int size;
static int step_counter = 0;

void copilot_init(const move_t * path_ptr, const int path_size)
{
    step_counter = 0;
    size = path_size;
    path = path_ptr;
}

void copilot_move(void)
{
    robot_status_t my_status = robot_get_status();
    pilot_start_move(&path[step_counter % size]);
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