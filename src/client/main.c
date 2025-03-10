/**
 * main program with pilot and robot modules
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../common/pilot_common.h"
#include "client.h"
#include "ui/ihm.h"
#include "ui/ui_logic.h"

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

int main(int argc, char *argv[])
{
  /* start the robot simulator and check its good starting */
  // TODO: Implement robot_start??
//  if (robot_start())
//  {
//    printf("Pb lancement mrPizz\n");
//    fflush(stdout);
//    return EXIT_FAILURE;
//  }
  printf("**** Client w/ GUI (GTK 3.0) **** \n");
  printf("**** par Tom & Marc **** \n");
  printf("Ctrl+C pour quitter\n");
  fflush(stdout);

  /* Ctrl+C to stop the program. */
  signal(SIGINT, sigint_handler);

  // Start client socket
  start_and_connect();

  /* main loop */
  app_loop();
  /* close the robot simulator */
  // TODO: Implement robot_close??
  // Close client socket
  stop_and_disconnect();
//  robot_close();
  return EXIT_SUCCESS;
}

/**
 * @brief Main loop for the application.
 * Send commands to the pilot and display robot's status with a specific period.
 */
static void app_loop()
{
  gtk_draw(0, NULL);
}
