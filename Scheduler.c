/* Scheduler.c 
 * 
 * Author   -   Nick Gorab
 * Created  -   5/12/2020
 * Modified -   5/12/2020
 *
 * This file creates the scheduler functionality 
 *
 */
#include "system.h"

task tasks[10] = {0, 0, 0, 0};
unsigned char max_tasks = 10;
unsigned char current_task = 0;

// This function can only create a soft definintion of the maximum number of tasks 
// There will always be a defined maximum that cannot be altered.
void setMaxTasks(int tasks) {
	max_tasks = tasks;
}

char getMaxTasks(void) {
	return max_tasks;
}



/* In order to prematurely stop this task, the function will return the unique task number
 * This can be stored by the function which calls the "Schedule_Task" function, allowing it
 * to keep track of which task is which, in the case that multiple tasks with the same function
 * are called at the same time.
 */

// TODO: Update this list to add the task to the next open task slot. 
// Issues can occur when a task is deleted which isn't the most recent task.
// 1, 2, 3, X, 4, 5
int Schedule_Task(void(*func)(), unsigned long period, char occurances) {
	tasks[current_task].period = period;
	tasks[current_task].TaskFunc = func;
	tasks[current_task].occurances = occurances;
	tasks[current_task].task_number = current_task;
	current_task++;
	return current_task;
}


void Remove_Task(unsigned char task_number) {
    tasks[task_number].period = 0;
    tasks[task_number].TaskFunc = 0;
    tasks[task_number].occurances = 0;
    tasks[task_number].task_number = 0;
    tasks[task_number].time_elapsed = 0;
}





