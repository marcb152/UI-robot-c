//
// Created by marc on 10/01/25.
//

#ifndef PILOT_H
#define PILOT_H

#include "../../common/pilot_common.h"

// Opaque pointer for the Pilot object
typedef struct Pilot Pilot;

// Constructor and destructor
Pilot* pilot_create(void);
void pilot_destroy(Pilot* pilot);

// Methods
void pilot_start_move(Pilot* pilot, const step_t *a_step);
move_status_t pilot_stop_at_target(Pilot* pilot);
move_status_t pilot_get_status(Pilot* pilot);
void pilot_stop(Pilot* pilot);

#endif // PILOT_H