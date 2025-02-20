//
// Created by marc on 10/01/25.
//

#ifndef PILOT_H
#define PILOT_H

#include "../../common/common.h"

void pilot_start_move(const step_t * a_step);

move_status_t pilot_stop_at_target(void);

move_status_t get_status(void);

void pilot_stop(void);

#endif //PILOT_H
