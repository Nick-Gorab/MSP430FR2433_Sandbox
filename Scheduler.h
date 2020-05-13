/* Scheduler.h
 * 
 * Author   -   Nick Gorab
 * Created  -   5/12/2020
 * Modified -   5/12/2020
 *
 * This header file creates the scheduler functionality 
 *
 */

typedef struct task {
	unsigned long period;
	unsigned long time_elapsed;
	unsigned char occurances;
	unsigned char task_number;
	void (*TaskFunc)(void);
} task;


void setMaxTasks(int tasks);
char getMaxTasks(void);
int Schedule_Task(void(*func)(), unsigned long period, char occurances); 
void Remove_Task(unsigned char task_number);

