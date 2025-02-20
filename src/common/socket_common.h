#ifndef SOCKET_COMMON_H
#define SOCKET_COMMON_H

#include "pilot_common.h"

#define PORT_DU_SERVEUR (12345)

typedef enum
{
  // Copilot init will send the total size of the array without data.
  // It will then call add step for each step in the path
  COPILOT_INIT,
  COPILOT_MOVE,
  COPILOT_IS_PATH_COMPLETE,
  COPILOT_ADD_STEP,
  COPILOT_GET_STEP,
  COPILOT_RM_STEP,
  COPILOT_DISPOSE,
  // Save and load commands will use the default file name server-side.
  // Thus no filename data is sent
  COPILOT_SAVE,
  COPILOT_LOAD,
  COPILOT_STOP,
} socket_command;

/**
 * @brief Structure that contains the data sent to and received bu the client/server.
 */
typedef struct
{
  socket_command command;
  // Optional, contains the step to add
  step_t step;
  // Optional, contains the index of the step to retrieve or the number of steps in the path
  int number;
} message_t;

#endif /* SOCKET_COMMON_H */
