// UI.c

#include "ui.h"
#include "copilot.h"
#include "pilot.h"
#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"

static int steps_number = 0;

// Menu options
typedef enum {
  CMD_CREATE_PATH,
  CMD_ADD_STEP,
  CMD_DESTROY_PATH,
  CMD_SHOW_PATH,
  CMD_START_PATH,
  CMD_QUIT,
  NB_COMMAND
} menu_command;

const char *menu_strings[] = {
    "Create a path",            // CMD_CREATE_PATH
    "Add a step to the Path",   // CMD_ADD_STEP
    "Destroy the path",         // CMD_DESTROY_PATH
    "Show the path",            // CMD_SHOW_PATH
    "Start to follow the path", // CMD_START_PATH
    "Quit"                      // CMD_START_QUIT
};

const char *success_messages[] = {
    "Path created",                     // CMD_CREATE_PATH
    "Step added successfully.",         // CMD_ADD_STEP
    "Path destroyed.",                  // CMD_PATH_DESTROYED
    "The current path is :",            // CMD_SHOW_PATH
    "Let's go \n press CTRL+C to exit", // CMD_START_PATH
    "Bye"                               // CMD_QUIT
};

const char *failure_messages[] = {
    "Failed to create path",                   // CMD_CREATE_PATH
    "Failed to add step, path isn't created.", // CMD_ADD_STEP
    "path already destroyed",                  // CMD_DESTROY_PATH
    "Path is not defined",                     // CMD_SHOW_PATH
    "Path is not defined",                     // CMD_START_PATH
    "" // No failure message for CMD_QUIT
};

typedef enum {
  UI_WELCOME = 0,
  UI_ASK_CHOICE,
  UI_ASK_SIZE_PATH,
  UI_ASK_STEP_NUMBER,
  UI_ASK_SPEED,
  UI_ASK_TYPE_MOVE,
  UI_ASK_VALUE_FORWARD,
  UI_ASK_VALUE_TURN,
  NB_PROMPT_MESSAGE
} id_message_prompt;

const char *prompt_messages[] = {
    "\n=== User Interface ===\n",
    "Enter your choice",
    "Enter the numer of step in the path",
    "Enter the step number",
    "Enter the speed",
    "Enter the type of move (0:Forward, 1:Rotation)",
    "Enter the distance to go forward",
    "Enter the type of rotation (1:left, 2:right, 3: flip turn)"};

static void print_prompt_message(id_message_prompt id) {
  printf("%s\n", prompt_messages[id]);
}

static void print_success_message(menu_command cmd) {
  printf("%s\n", success_messages[cmd]);
}

static void print_failure_message(menu_command cmd) {
  printf("%s\n", failure_messages[cmd]);
}

static void print_menu() {
  print_prompt_message(UI_WELCOME);
  for (int i = 0; i < NB_COMMAND; i++) {
    printf("%d. %s\n", i, menu_strings[i]);
  }
}

static int handle_user_prompt_int(id_message_prompt id, int min, int max) {
  int value = 0;
  do {
    print_prompt_message(id);
    printf("Enter a value between %d and %d\n", min, max);
    scanf("%d", &value); // version cracra et non sécurisée
    getchar();           // pour se débarasser du retour ligne
  } while ((value < min) || (value > max));
  return value;
}

static void handle_create_path() {
  /*TODO: prompt for the number of steps and create an empty path, print success
   * or failure messages accordingly */
  /*HINT: update steps_number on success or set to 0 on failure */
}

static void handle_add_step() {
  /*TODO: prompt for adequate data to call copilot_add_step(index,step); */
  /*HINT: adding step fails if steps_number == 0 */
}

static void handle_destroy_path() {
  /*TODO: call copilot_destroy_path(); */
  /*HINT: steps_number is set to 0 on success to prevent adding steps to a
   * non-existing path */
}

static int handle_start_path() {
  /*TODO: exit ui to start path */
  /*HINT: how could you detect an empty path? (created but with no added steps?)
   */
}

static void handle_show_path() {
  int i = 0;
  move step;
  if (steps_number > 0) {
    print_success_message(CMD_SHOW_PATH);
    for (i = 0; i < steps_number; i++) {
      step = copilot_get_step(i);
      if (step.speed != 0) {
        if (step.type == FORWARD)
          printf("%d : FORWARD speed:%d distance:%d \n", i, step.speed,
                 step.range.distance);
        else
          printf("%d : ROTATION speed : %d turn: %d \n", i, step.speed,
                 step.range.angle);
      } else {
        printf("%d : No MOVE\n", i);
      }
    }
  } else {
    print_failure_message(CMD_SHOW_PATH);
  }
}

extern int ui_start() {
  int choice;
  // User interface loop
  while (1) {
    print_menu();
    choice = handle_user_prompt_int(UI_ASK_CHOICE, 0, (NB_COMMAND - 1));
    switch (choice) {
    case CMD_CREATE_PATH:
      handle_create_path();
      break;
    case CMD_ADD_STEP:
      handle_add_step();
      break;
    case CMD_DESTROY_PATH:
      handle_destroy_path();
      break;
    case CMD_SHOW_PATH:
      handle_show_path();
      break;
    case CMD_START_PATH:
      if (handle_start_path() == EXIT_SUCCESS)
        return EXIT_SUCCESS;
      break;
    case CMD_SAVE_PATH:
      handle_save_path();
      break;
    case CMD_LOAD_PATH:
      handle_load_path();
      break;
    case CMD_QUIT:
      print_success_message(CMD_QUIT);
      return EXIT_FAILURE;
    default:
      break;
    }
  }
  return EXIT_SUCCESS; // never used
}
