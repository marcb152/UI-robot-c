/**
 * main program with pilot and robot modules
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "server.h"
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

static void* robot_loop(void* arg);

/**
 * @brief Global variable used for program clean exit
 */
static process_state_t running = ALIVE;

static pthread_t robot_thread;

static volatile bool robot_moving = false;

/**
 * @brief Function for CTRL+C signal management
 */
static void sigint_handler(int dummy) { running = STOPPED; }

int main(int argc, char *argv[])
{
  /* start the robot simulator and check its good starting */
  if (robot_start())
  {
    printf("Pb lancement mrPizz\n");
    fflush(stdout);
    return EXIT_FAILURE;
  }
  printf("**** Serveur pour Robot **** \n");
  printf("**** par Marc & Tom **** \n");
  printf("Ctrl+C pour quitter\n");
  fflush(stdout);

  /* Ctrl+C to stop the program. */
  signal(SIGINT, sigint_handler);

  // Start server socket
  start_and_connect();

  /* main loop */
  app_loop();

  // Close server socket
  stop_and_disconnect();

  // Clean up copilot resources
  copilot_cleanup();

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
  while (running)
  {
    int data = communication_avec_client();
    switch (data)
    {
      // 0 = client disconnected, exit server
      case 0:
        running = 0;
        return;
      // 1 = default, everything fine
      case 1:
        break;
      case START_MOVING:
        // Starts the robot thread
        robot_moving = true;
        pthread_create(&robot_thread, NULL, &robot_loop, NULL);
        pthread_detach(robot_thread);
        break;
      case STOP_MOVING:
        // Graceful stop of the robot
        robot_moving = false;
        // Stops the robot thread from moving
        if (robot_thread) pthread_cancel(robot_thread);
        // Stop the robot
        copilot_stop();
        break;
    }
  }
}

/**
 * @brief Main loop for the robot.
 * Send commands to the pilot and display robot's status with a specific period.
 */
static void* robot_loop(void* arg)
{
  	while (robot_moving)
    {
        copilot_move();
        if (copilot_is_path_complete())
        {
            copilot_stop();
            break;
        }
    }
    pthread_exit(NULL);
}
