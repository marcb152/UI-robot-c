//
// Created by marc on 10/01/25.
//

#include "pilot.h"

#include <stdlib.h>

#include "robot.h"

#define TURN_SENSOR_ROTATION 450

static move_status_t robot_moving;
static int target_pos;
static wheel_t reference_wheel;

void pilot_start_move(const step_t *a_step)
{
    robot_reset_wheel_pos();
    switch (a_step->move.action)
    {
        case FORWARD:
            robot_moving = MOVE_FORWARDING;
            robot_set_speed(a_step->speed, a_step->speed);
            reference_wheel = LEFT_WHEEL;
            target_pos = a_step->move.distance * 10;
            break;
        case ROTATION:
            robot_moving = MOVE_TURNING;
            switch (a_step->move.angle)
            {
                case LEFT:
                    robot_set_speed(0, a_step->speed);
                    reference_wheel = RIGHT_WHEEL;
                    target_pos = TURN_SENSOR_ROTATION;
                    break;
                case RIGHT:
                    robot_set_speed(a_step->speed, 0);
                    reference_wheel = LEFT_WHEEL;
                    target_pos = TURN_SENSOR_ROTATION;
                    break;
                case U_TURN:
                    robot_set_speed(a_step->speed, 0);
                    reference_wheel = LEFT_WHEEL;
                    target_pos = 2 * TURN_SENSOR_ROTATION;
                    break;
                default:
                    if (a_step->move.angle <= 0)
                    {
                        reference_wheel = RIGHT_WHEEL;
                        robot_set_speed(0, a_step->speed);
                    }
                    else
                    {
                        reference_wheel = LEFT_WHEEL;
                        robot_set_speed(a_step->speed, 0);
                    }
                    target_pos = abs(a_step->move.angle) * TURN_SENSOR_ROTATION / 90;
            }
            break;
    }
}

move_status_t pilot_stop_at_target(void)
{
    if (robot_get_wheel_position(reference_wheel) >= target_pos)
    {
        robot_moving = MOVE_DONE;
    }
    return robot_moving;
}

move_status_t get_status(void)
{
    return robot_moving;
}
