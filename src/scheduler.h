#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <sys/types.h>

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

void create_process(Priority priority, int arrival_time, int execution_time, int printers, int scanners, int modems, int cds);
void delete_process(Process* process);
void dispatch_process(Process* process);
void execute_process(Process* process);
bool validate_resources(int needed, int available);

#endif
