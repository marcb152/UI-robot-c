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

void copilot_init(const move_t * path, const int size);

void copilot_move(void);

int copilot_is_path_complete(void);

#endif //COPILOT_H
