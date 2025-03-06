#include "pilot.h"
#include <stdlib.h>
#include <assert.h>
#include "robot.h"

#define TURN_SENSOR_ROTATION 450

// Define states, events and actions enums
typedef enum {
    S_IDLE,
    S_FORWARDING,
    S_TURNING
} state_t;

typedef enum {
    E_FORWARD,
    E_ROTATE,
    E_CHECK_POS,
    E_STOP
} event_t;

typedef enum {
    A_NOP,
    A_START_FORWARD,
    A_START_TURN,
    A_CHECK_POSITION,
    A_STOP_ROBOT
} action_t;

// State transition structure
typedef struct {
    state_t next_state;
    action_t action;
} transition_t;

// Define the Pilot object structure
struct Pilot {
    state_t state;
    int target_pos;
    wheel_t reference_wheel;
    const step_t *current_step;
};

// State transition table
static const transition_t transition_table[3][4] = {
    // S_IDLE
    {
        [E_FORWARD]   = {S_FORWARDING, A_START_FORWARD},
        [E_ROTATE]    = {S_TURNING, A_START_TURN},
        [E_CHECK_POS] = {S_IDLE, A_NOP},
        [E_STOP]      = {S_IDLE, A_STOP_ROBOT}
    },
    // S_FORWARDING
    {
        [E_FORWARD]   = {S_FORWARDING, A_START_FORWARD},
        [E_ROTATE]    = {S_TURNING, A_START_TURN},
        [E_CHECK_POS] = {S_FORWARDING, A_CHECK_POSITION}, // May change to S_IDLE inside handler
        [E_STOP]      = {S_IDLE, A_STOP_ROBOT}
    },
    // S_TURNING
    {
        [E_FORWARD]   = {S_FORWARDING, A_START_FORWARD},
        [E_ROTATE]    = {S_TURNING, A_START_TURN},
        [E_CHECK_POS] = {S_TURNING, A_CHECK_POSITION}, // May change to S_IDLE inside handler
        [E_STOP]      = {S_IDLE, A_STOP_ROBOT}
    }
};

// Function to process events through the state machine
static void pilot_process_event(Pilot* pilot, event_t event);

// Constructor
Pilot* pilot_create(void) {
    Pilot* pilot = (Pilot*)malloc(sizeof(Pilot));
    if (pilot) {
        pilot->state = S_IDLE;
        pilot->target_pos = 0;
        pilot->reference_wheel = LEFT_WHEEL;
        pilot->current_step = NULL;
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
    pilot->current_step = a_step;

    if (a_step->move.action == FORWARD) {
        pilot_process_event(pilot, E_FORWARD);
    } else {
        pilot_process_event(pilot, E_ROTATE);
    }
}

move_status_t pilot_stop_at_target(Pilot* pilot) {
    pilot_process_event(pilot, E_CHECK_POS);

    // Map state to move_status_t
    switch (pilot->state) {
        case S_IDLE:       return MOVE_DONE;
        case S_FORWARDING: return MOVE_FORWARDING;
        case S_TURNING:    return MOVE_TURNING;
        default:           return MOVE_DONE;
    }
}

move_status_t pilot_get_status(Pilot* pilot) {
    switch (pilot->state) {
        case S_IDLE:       return MOVE_DONE;
        case S_FORWARDING: return MOVE_FORWARDING;
        case S_TURNING:    return MOVE_TURNING;
        default:           return MOVE_DONE;
    }
}

void pilot_stop(Pilot* pilot) {
    pilot_process_event(pilot, E_STOP);
}

// Process event and execute actions according to state machine
static void pilot_process_event(Pilot* pilot, event_t event) {
    state_t current_state = pilot->state;
    action_t transition_action = transition_table[current_state][event].action;
    state_t destination_state = transition_table[current_state][event].next_state;

    // Execute action
    switch (transition_action) {
        case A_NOP:
            break;

        case A_START_FORWARD: {
            robot_reset_wheel_pos();
            robot_set_speed(pilot->current_step->speed, pilot->current_step->speed);
            pilot->reference_wheel = LEFT_WHEEL;
            pilot->target_pos = pilot->current_step->move.distance * 10;
            break;
        }

        case A_START_TURN: {
            robot_reset_wheel_pos();
            int angle = pilot->current_step->move.angle;

            if (angle == LEFT) {
                robot_set_speed(0, pilot->current_step->speed);
                pilot->reference_wheel = RIGHT_WHEEL;
                pilot->target_pos = TURN_SENSOR_ROTATION;
            } else if (angle == RIGHT) {
                robot_set_speed(pilot->current_step->speed, 0);
                pilot->reference_wheel = LEFT_WHEEL;
                pilot->target_pos = TURN_SENSOR_ROTATION;
            } else if (angle == U_TURN) {
                robot_set_speed(pilot->current_step->speed, 0);
                pilot->reference_wheel = LEFT_WHEEL;
                pilot->target_pos = 2 * TURN_SENSOR_ROTATION;
            } else {
                if (angle <= 0) {
                    pilot->reference_wheel = RIGHT_WHEEL;
                    robot_set_speed(0, pilot->current_step->speed);
                } else {
                    pilot->reference_wheel = LEFT_WHEEL;
                    robot_set_speed(pilot->current_step->speed, 0);
                }
                pilot->target_pos = abs(angle) * TURN_SENSOR_ROTATION / 90;
            }
            break;
        }

        case A_CHECK_POSITION:
            if (robot_get_wheel_position(pilot->reference_wheel) >= pilot->target_pos) {
                destination_state = S_IDLE; // Target reached, transition to IDLE
                robot_set_speed(0, 0);
            }
            break;

        case A_STOP_ROBOT:
            robot_set_speed(0, 0);
            break;

        default:
            assert(0); // Invalid action
            break;
    }

    // Update state
    pilot->state = destination_state;
}