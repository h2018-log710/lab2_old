#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <sys/types.h>
#include "list.h"

typedef enum Priority
{
	REAL_TIME = 0,
	USER_HIGH = 1,
	USER_NORMAL = 2,
	USER_LOW = 3
} Priority;

typedef struct Process
{
	pid_t pid;
	Priority priority;
	int arrival_time;
	int execution_time;
	int remaining_time;
	int printers;
	int scanners;
	int modems;
	int cds;
	bool is_paused;
	bool is_running;
} Process;

void create_process(int arrival_time, Priority priority, int execution_time, int printers, int scanners, int modems, int cds);
void dispatch_process(int tick);
void execute_process(Process* process);
void admit_process(Process* process);
void print_process(Process* process);
bool dispatch_waiting_process();
void manage_process(List* list, Process** process, Priority priority);
void initialize_scheduler();
void start_scheduler();

#endif
