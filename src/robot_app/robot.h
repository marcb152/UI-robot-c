#ifndef ROBOT_H
#define ROBOT_H

/**
 * @file robot.h
 * @brief Declaration of the Robot class.
 */

// @todo : Define the different types needed

typedef int speed_pct_t;

typedef enum
{
    LEFT_WHEEL = 0,
    RIGHT_WHEEL = 1,
} wheel_t;

typedef struct
{
    int left_encoder;
    int right_encoder;
    int left_sensor;
    int right_sensor;
    int center_sensor;
    int battery;
} robot_status_t;

/**
 * @brief Notification sur l'état du robot
 */
typedef enum
{
    ROBOT_OK = 0,
    ROBOT_IDLE = 1,
    ROBOT_PROBLEM = 2,
    ROBOT_OBSTACLE = 3,
} notification_t;

/**
 * @brief Initializes and starts the robot.
 *
 * @return The initialization status.
 */
int robot_start(void);

/**
 * @brief Sets the speed of the robot's wheels.
 *
 */
void robot_set_speed(speed_pct_t left, speed_pct_t right);

/**
 * @brief Gets the position of a specific wheel.
 *
 * @param wheel_id
 * @return The position of the specified wheel.
 */
int robot_get_wheel_position(wheel_t wheel_id);

/**
 * @brief Resets the position of the robot's wheels.
 */
void robot_reset_wheel_pos(void);

/**
 * @brief Gets the current status of the robot.
 *
 * @return The status of the robot.
 */
robot_status_t robot_get_status(void);

/**
 * @brief Signals an event to external users.
 *
 * @param event The event notification.
 */
void robot_signal_event(notification_t event);

/**
 * @brief Stops the robot and close the link to the robot.
 */
void robot_close(void);

#endif // ROBOT_H
