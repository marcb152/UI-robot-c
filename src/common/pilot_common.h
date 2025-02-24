//
// Created by marc on 20/02/25.
//

#ifndef PILOT_COMMON_H
#define PILOT_COMMON_H

#define FORWARD 0
#define ROTATION 1
#define RIGHT 90
#define LEFT -90
#define U_TURN 180

typedef enum
{
    MOVE_DONE,
    MOVE_TURNING,
    MOVE_FORWARDING,
    MOVE_OBSTACLE_FORWARD,
} move_status_t;

/**
 * @brief Structure to represent a movement action.
 */
typedef struct
{
    /** The action to be performed (e.g., FORWARD, ROTATION). */
    int action;
    /** The angle for rotation actions (e.g., RIGHT, LEFT, U_TURN). */
    int angle;
    /** The distance for forward actions. */
    int distance;
} move_t;

/** @brief Structure representing a step in the path.
 * A step is composed of a move and a speed.
 */
typedef struct
{
    move_t move;
    int speed;
} step_t;

#endif //PILOT_COMMON_H
