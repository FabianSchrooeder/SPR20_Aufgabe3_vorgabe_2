#ifndef RR_H
#define RR_H
#include "queue.h"
#include "process.h"
#include <stdlib.h>

queue_object* RR_queue;
int this_quantum;
int time_left_quantum;
//You can add more global variables and structs here

/**
 * determine which is the current process to be processed. 
 * @param running_process the prevously running process
 * @result a pointer to the new running process (if something changed) or to the previously running process
 */ 
process* RR_tick (process* running_process);

/**
 * Do everything you have to at startup in this function. It is called once.
 * @param quantum the specified (last parameter in program call) or the default quantum
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int RR_startup(int quantum);

/**
 * Handle new arrivals
 * @param arriving_process A pointer to a new arriving process
 * @param running_process A pointer to previously running process
 * @result A pointer to the new active (running) process (if that changed) or to the previously running process
 */ 
process* RR_new_arrival(process* arriving_process, process* running_process);

/**
 * is called once after the all processes were handled. In case you want to cleanup something
 */
void RR_finish();
#endif //RR_H