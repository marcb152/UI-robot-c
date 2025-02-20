// UI.c

#include "ui_logic.h"

#include "../../common/pilot_common.h"
#include "../client.h"
#include <stdio.h>
#include <stdlib.h>

static int steps_number = 0;

// Menu options
typedef enum {
  CMD_CREATE_PATH,
  CMD_ADD_STEP,
  CMD_DESTROY_PATH,
  CMD_SHOW_PATH,
  CMD_START_PATH,
  CMD_SAVE_PATH,
  CMD_LOAD_PATH,
  CMD_QUIT,
  NB_COMMAND,
} menu_command;

const char *menu_strings[] = {
    "Create a path",            // CMD_CREATE_PATH
    "Add a step to the Path",   // CMD_ADD_STEP
    "Destroy the path",         // CMD_DESTROY_PATH
    "Show the path",            // CMD_SHOW_PATH
    "Start to follow the path", // CMD_START_PATH
    "Save path to file",        // CMD_SAVE_PATH
    "Load path from file",      // CMD_LOAD_PATH
    "Quit"                      // CMD_QUIT
};

const char *success_messages[] = {
    "Path created",                     // CMD_CREATE_PATH
    "Step added successfully.",         // CMD_ADD_STEP
    "Path destroyed.",                  // CMD_PATH_DESTROYED
    "The current path is :",            // CMD_SHOW_PATH
    "Let's go \n press CTRL+C to exit", // CMD_START_PATH
    "Path saved to file",               // CMD_SAVE_PATH
    "Path loaded from file",            // CMD_LOAD_PATH
    "Bye"                               // CMD_QUIT
};

const char *failure_messages[] = {
    "Failed to create path",                   // CMD_CREATE_PATH
    "Failed to add step, path isn't created.", // CMD_ADD_STEP
    "path already destroyed",                  // CMD_DESTROY_PATH
    "Path is not defined",                     // CMD_SHOW_PATH
    "Path is not defined",                     // CMD_START_PATH
    "Saving failed",                           // CMD_SAVE_PATH
    "Loading failed",                          // CMD_LOAD_PATH
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

  int temp = handle_user_prompt_int(UI_ASK_SIZE_PATH, 1, 100);
  step_t *path = calloc(temp, sizeof(step_t));
  if (temp > 0) {
    // Initialize copilot with the path and the number of steps
    copilot_init(path, temp);
    steps_number = temp;
    print_success_message(CMD_CREATE_PATH);
  } else {
    steps_number = 0;
    print_failure_message(CMD_CREATE_PATH);
  }
}

static void handle_add_step() {
  /*TODO: prompt for adequate data to call copilot_add_step(index,step); */
  /*HINT: adding step fails if steps_number == 0 */

  if (steps_number == 0) {
    print_failure_message(CMD_ADD_STEP);
    return;
  }
  int step_nbr = handle_user_prompt_int(UI_ASK_STEP_NUMBER, 0, steps_number);
  int speed = handle_user_prompt_int(UI_ASK_SPEED, 1, 100);
  int type = handle_user_prompt_int(UI_ASK_TYPE_MOVE, 0, 1);
  int dst_or_angle =
    type == FORWARD ?
      handle_user_prompt_int(UI_ASK_VALUE_FORWARD, 0, 100) :
      handle_user_prompt_int(UI_ASK_VALUE_TURN, 1, 3);
  // We define the same value for distance and angle since we only need one of them
  // Also because I don't care
  step_t step = { {type, dst_or_angle, dst_or_angle}, speed};
  // If the user wants to add a step at the end of the path that doesn't exist yet
  if (step_nbr == steps_number) {
    steps_number++;
  }
  copilot_add_step(step_nbr, &step);
  print_success_message(CMD_ADD_STEP);
}

static void handle_rm_step()
{
  if (steps_number == 0) {
    print_failure_message(CMD_ADD_STEP);
    return;
  }
  int step_nbr = handle_user_prompt_int(UI_ASK_STEP_NUMBER, 0, steps_number - 1);
  copilot_rm_step(step_nbr);
  steps_number--;
  print_success_message(CMD_DESTROY_PATH);
}

static void handle_destroy_path() {
  /*TODO: call copilot_destroy_path(); */
  /*HINT: steps_number is set to 0 on success to prevent adding steps to a
   * non-existing path */
  copilot_dispose();
  steps_number = 0;
  print_success_message(CMD_DESTROY_PATH);
}

static int handle_start_path() {
  /*TODO: exit ui to start path */
  /*HINT: how could you detect an empty path? (created but with no added steps?)
   */
  if (steps_number <= 0)
  {
    print_failure_message(CMD_START_PATH);
    return EXIT_FAILURE;
  }
  // We check that every step as a defined speed
  int i = 0;
  step_t * step;
  for (i = 0; i < steps_number; i++)
  {
    step = copilot_get_step(i);
    if (step->speed == 0)
    {
      print_failure_message(CMD_START_PATH);
      return EXIT_FAILURE;
    }
  }
  // Every check passed, we can start the path
  return EXIT_SUCCESS;
}

static void handle_show_path() {
  int i = 0;
  step_t * step;
  if (steps_number > 0) {
    print_success_message(CMD_SHOW_PATH);
    for (i = 0; i < steps_number; i++) {
      step = copilot_get_step(i);
      if (step->speed != 0) {
        if (step->move.action == FORWARD)
          printf("%d : FORWARD speed:%d distance:%d \n", i, step->speed,
                 step->move.distance);
        else
          printf("%d : ROTATION speed : %d turn: %d \n", i, step->speed,
                 step->move.angle);
      } else {
        printf("%d : No MOVE\n", i);
      }
    }
  } else {
    print_failure_message(CMD_SHOW_PATH);
  }
}

static void handle_save_path()
{
  if (copilot_save("path.txt") != -1)
  {
    print_success_message(CMD_SAVE_PATH);
  }
  else
  {
    print_failure_message(CMD_SAVE_PATH);
  }
}

static void handle_load_path()
{
  const int temp = copilot_load("path.txt");
  if (temp != -1)
  {
    steps_number = temp;
    print_success_message(CMD_LOAD_PATH);
  }
  else
  {
    print_failure_message(CMD_LOAD_PATH);
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
      // DONE
      handle_create_path();
      break;
    case CMD_ADD_STEP:
      // DONE
      handle_add_step();
      break;
    case CMD_DESTROY_PATH:
      // TODO
      handle_destroy_path();
      // DONE
      handle_rm_step();
      break;
    case CMD_SHOW_PATH:
      // DONE
      handle_show_path();
      break;
    case CMD_START_PATH:
      // TODO
      if (handle_start_path() == EXIT_SUCCESS)
        return EXIT_SUCCESS;
      break;
    case CMD_SAVE_PATH:
      // TODO
      handle_save_path();
      break;
    case CMD_LOAD_PATH:
      // TODO
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
