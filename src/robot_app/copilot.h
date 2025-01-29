//
// Created by marc on 23/01/25.
//

#ifndef COPILOT_H
#define COPILOT_H

#include "pilot.h"

/** @brief Max scanning's attempt to check if a move is finished.
 * Adapt according your move
 */
#define ENCODERS_SCAN_NB 1500

/** @brief Waiting time between two encoder's scans (in microseconds).
 * Adapt according to your cpu and simulator.
 */
#define DELAY 100000

typedef struct
{
    move_t move;
    int speed;
} step_t;

void copilot_init(step_t * path, int size);

void copilot_move(void);

int copilot_is_path_complete(void);

void copilot_add_step(int index, step_t * step);

step_t * copilot_get_step(int i);

void copilot_rm_step(int index);

#endif //COPILOT_H
