//
// Created by marc on 10/01/25.
//

#include "pilot.h"

#include "robot.h"

#define TURN_SENSOR_ROTATION 450
#define FULL_SPEED 100

static move_status_t robot_moving;
static int target_pos;
static wheel_t reference_wheel;

void pilot_start_move(const move_t *a_move)
{
    robot_reset_wheel_pos();
    switch (a_move->action)
    {
        case FORWARD:
            robot_moving = MOVE_FORWARDING;
            robot_set_speed(FULL_SPEED, FULL_SPEED);
            reference_wheel = LEFT_WHEEL;
            target_pos = a_move->distance * 10;
            break;
        case ROTATION:
            robot_moving = MOVE_TURNING;
            switch (a_move->angle)
            {
                case LEFT:
                    robot_set_speed(0, FULL_SPEED);
                    reference_wheel = RIGHT_WHEEL;
                    target_pos = TURN_SENSOR_ROTATION;
                    break;
                case RIGHT:
                    robot_set_speed(FULL_SPEED, 0);
                    reference_wheel = LEFT_WHEEL;
                    target_pos = TURN_SENSOR_ROTATION;
                    break;
                case U_TURN:
                    robot_set_speed(FULL_SPEED, 0);
                    reference_wheel = LEFT_WHEEL;
                    target_pos = 2 * TURN_SENSOR_ROTATION;
                    break;
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
