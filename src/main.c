/**
 * main program with pilot and robot modules
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "robot_app/pilot.h"
#include "robot_app/robot.h"
#include "robot_app/copilot.h"

/**
 * @mainpage Robot application mission 2.
 * This projects aims to move a robot along a predefined trajectory.
 */

/**
 * @enum process_state_t
 * @brief indicate the process state management for the application.
 */
typedef enum {
  STOPPED = 0, /**< Process is stopped */
  ALIVE        /**< Process is running */
} process_state_t;

// declaration of private functions
static void app_loop(void);

/**
 * @brief Global variable used for program clean exit
 */
static process_state_t running = ALIVE;

/**
 * @brief Function for CTRL+C signal management
 */
static void sigint_handler(int dummy) { running = STOPPED; }

int main(void)
{
  /* start the robot simulator and check its good starting */
  if (robot_start())
  {
    printf("Pb lancement mrPizz\n");
    fflush(stdout);
    return EXIT_FAILURE;
  }
  printf("**** Version démo RM2 **** \n");
  printf("**** par JDL **** \n");
  printf("Ctrl+C pour quitter\n");
  fflush(stdout);

  /* Ctrl+C to stop the program. */
  signal(SIGINT, sigint_handler);
  /* main loop */
  app_loop();
  /* close the robot simulator */
  robot_close();
  return EXIT_SUCCESS;
}

/**
 * @brief Main loop for the application.
 * Send commands to the pilot and display robot's status with a specific period.
 */

static void app_loop()
{
  int size = 6;
  // move_t * path_ptr = calloc(size, sizeof(move_t));
  // path_ptr[0] = (move_t){FORWARD, {5}, 30};
  // path_ptr[1] = (move_t){ROTATION, {RIGHT}, 20};
  // path_ptr[2] = (move_t){FORWARD, {5}, 30};
  // path_ptr[3] = (move_t){ROTATION, {LEFT}, 20};
  // path_ptr[4] = (move_t){FORWARD, {5}, 30};
  // path_ptr[5] = (move_t){ROTATION, {U_TURN}, 20};
  // copilot_init(path_ptr, size);

  while (running)
  {
    copilot_move();
    if (copilot_is_path_complete())
    {
      size = 8;
      // reallocarray(path_ptr, size, sizeof(move_t));
      // path_ptr[0] = (move_t){FORWARD, {5}, 30};
      // path_ptr[1] = (move_t){ROTATION, {RIGHT}, 20};
      // path_ptr[2] = (move_t){FORWARD, {5}, 30};
      // path_ptr[3] = (move_t){ROTATION, {LEFT}, 20};
      // path_ptr[4] = (move_t){FORWARD, {5}, 30};
      // path_ptr[5] = (move_t){ROTATION, {U_TURN}, 20};
      // path_ptr[6] = (move_t){FORWARD, {5}, 30};
      // path_ptr[7] = (move_t){ROTATION, {LEFT}, 20};
      // copilot_init(path_ptr, size);
    }
  }
}
