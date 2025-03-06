//
// Created by marc on 10/01/25.
//

#include "pilot.h"

#include <stdlib.h>

#include "robot.h"

#define TURN_SENSOR_ROTATION 450

// Define the Pilot object structure
struct Pilot {
    move_status_t robot_moving;
    int target_pos;
    wheel_t reference_wheel;
};

// Constructor
Pilot* pilot_create(void) {
    Pilot* pilot = (Pilot*)malloc(sizeof(Pilot));
    if (pilot) {
        pilot->robot_moving = MOVE_DONE;  // Assume we start in a stopped state
        pilot->target_pos = 0;
        pilot->reference_wheel = LEFT_WHEEL;
    }
    return pilot;
}

// Destructor
void pilot_destroy(Pilot* pilot) {
    if (pilot) {
        free(pilot);
    }
}

void pilot_start_move(Pilot* pilot, const step_t *a_step) {
    robot_reset_wheel_pos();
    switch (a_step->move.action) {
        case FORWARD:
            pilot->robot_moving = MOVE_FORWARDING;
            robot_set_speed(a_step->speed, a_step->speed);
            pilot->reference_wheel = LEFT_WHEEL;
            pilot->target_pos = a_step->move.distance * 10;
            break;
        case ROTATION:
            pilot->robot_moving = MOVE_TURNING;
            switch (a_step->move.angle) {
                case LEFT:
                    robot_set_speed(0, a_step->speed);
                    pilot->reference_wheel = RIGHT_WHEEL;
                    pilot->target_pos = TURN_SENSOR_ROTATION;
                    break;
                case RIGHT:
                    robot_set_speed(a_step->speed, 0);
                    pilot->reference_wheel = LEFT_WHEEL;
                    pilot->target_pos = TURN_SENSOR_ROTATION;
                    break;
                case U_TURN:
                    robot_set_speed(a_step->speed, 0);
                    pilot->reference_wheel = LEFT_WHEEL;
                    pilot->target_pos = 2 * TURN_SENSOR_ROTATION;
                    break;
                default:
                    if (a_step->move.angle <= 0) {
                        pilot->reference_wheel = RIGHT_WHEEL;
                        robot_set_speed(0, a_step->speed);
                    } else {
                        pilot->reference_wheel = LEFT_WHEEL;
                        robot_set_speed(a_step->speed, 0);
                    }
                    pilot->target_pos = abs(a_step->move.angle) * TURN_SENSOR_ROTATION / 90;
            }
            break;
    }
}

move_status_t pilot_stop_at_target(Pilot* pilot) {
    if (robot_get_wheel_position(pilot->reference_wheel) >= pilot->target_pos) {
        pilot->robot_moving = MOVE_DONE;
    }
    return pilot->robot_moving;
}

move_status_t pilot_get_status(Pilot* pilot) {
    return pilot->robot_moving;
}

void pilot_stop(Pilot* pilot) {
    pilot->robot_moving = MOVE_DONE;
    robot_set_speed(0, 0);
}