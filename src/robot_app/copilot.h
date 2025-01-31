//
// Created by marc on 23/01/25.
//

#ifndef COPILOT_H
#define COPILOT_H

#include "pilot.h"

/** @brief Max scanning's attempt to check if a move is finished.
 * Adapt according your move
 */
#define ENCODERS_SCAN_NB 3500

/** @brief Waiting time between two encoder's scans (in microseconds).
 * Adapt according to your cpu and simulator.
 */
#define DELAY 100000

/**
 * Initializes the copilot with the given path and size.
 *
 * @param path Pointer to the array of steps.
 * @param size Number of steps in the path.
 */
void copilot_init(step_t * path, int size);

/**
 * @brief Moves the robot according to the current step in the path.
 *
 * This function retrieves the current status of the robot, starts the movement
 * for the current step, and prints the status of the encoders, sensors, and battery.
 * It then polls the encoders to check if the movement is complete and increments
 * the step counter if the movement is done.
 *
 * Must be called in a loop to work properly.
 */
void copilot_move(void);

/**
 * @brief Checks if the copilot has completed the path.
 *
 * It checks if the current index equals the size of the path.
 *
 * @return 1 if the path is complete, 0 otherwise.
 */
int copilot_is_path_complete(void);

/**
 * @brief Adds a step to the path at the specified index.
 *
 * This function inserts a step into the path at the given index.
 * WARNING: If a step already exists at the specified index, it will be overwritten.
 *
 * @param index The index at which to add the step.
 * @param step A pointer to the step to add.
 */
void copilot_add_step(int index, step_t * step);

/**
 * @brief Retrieves a step from the path.
 *
 * This function returns a pointer to the step at the specified index in the path.
 *
 * @param i The index of the step to retrieve.
 * @return A pointer to the step at the specified index, if it exists, NULL otherwise.
 */
step_t * copilot_get_step(int i);

/**
 * @brief Removes a step from the path.
 *
 * This function removes the step at the specified index from the path.
 *
 * @param index The index of the step to remove.
 */
void copilot_rm_step(int index);

/**
 * @brief Frees resources allocated by the copilot.
 *
 * This function should be called to clean up any resources that were allocated
 * during the copilot's operation. It ensures that memory is properly freed and
 * any other necessary cleanup is performed.
 */
void copilot_dispose(void);

/**
 * @brief Saves the current path to a file.
 *
 * This function writes the current path to a file in the following format:
 * 1. Number of steps
 * 2. For each step:
 *    - Type of move
 *    - Speed
 *    - Distance or angle (based on type)
 *
 * @param filename The name of the file to save the path to.
 * @return 0 on success, or -1 if an error occurred.
 */
int copilot_save(char * filename);

/**
 * @brief Loads a path from a file.
 *
 * This function reads a file containing a series of steps and initializes the copilot with the loaded path.
 * The file format is expected to be:
 * 1. Number of steps
 * 2. For each step:
 *    - Type of move
 *    - Speed
 *    - Distance or angle (based on type)
 *
 * @param filename The name of the file to load the path from.
 * @return The number of steps loaded, or -1 if an error occurred.
 */
int copilot_load(char * filename);

#endif //COPILOT_H
