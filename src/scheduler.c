#include "scheduler.h"
#include "list.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static const int QUANTUM = 1;

static List* incoming_process_list = NULL;		// Incoming processes yet to be dispatched.
static List* real_time_process_list = NULL;		// Real-time processes ready to be executed.
static List* user_high_process_list = NULL;		// User processes with high priority ready to be executed.
static List* user_normal_process_list = NULL;	// User processes with normal priority ready to be executed.
static List* user_low_process_list = NULL;		// User processes with low priority ready to be executed.
static List* user_wait_process_list = NULL;		// User processes waiting for resources to be available.

void create_process(Priority priority, int arrival_time, int execution_time, int printers, int scanners, int modems, int cds)
{
	Process* process = malloc(sizeof(Process));
	
	process.pid = 0;
	process.priority = priority;
	process.arrival_time = arrival_time;
	process.execution_time = execution_time;
	process.remaining_time = execution_time;
	process.printers = printers;
	process.scanners = scanners;
	process.modems = modems;
	process.cds = cds;
	process.is_paused = false;
	process.is_running = false;
	
	push_back(incoming_process_list, process);
}

void delete_process(Node* list, Process* process)
{
	remove(list, process); // FIX
	free(process);
}

void dispatch_process(Process* process)
{
	switch (process->priority)
	{
		case REAL_TIME:
			push_back(real_time_process_list, process);
			break;
		default:
			switch(process->priority) // TODO Dispatch accordingly [push_back / push_front / insert].
			{
				case USER_HIGH:
					push_back(user_high_process_list, process);
					break;
				case USER_NORMAL:
					push_back(user_normal_process_list, process);
					break;
				case USER_LOW:
					push_back(user_low_process_list, process);
					break;
				default:
					printf("Invalid process priority.\n");
					exit(-1);
					break;
			}
			break;
	}
}

void execute_process(Process* process)
{
	process->pid = fork();
	
	if (process->pid == -1)
    {
        printf("Failed to fork.\n");
        exit(-1);
    }
    
    else if (process->pid == 0)
    {
		char* arguments[2];
		arguments[0] = "./log710h15process";
		arguments[1] = NULL;
		
		process->pid = getpid();
		
        if (execvp(arguments[0], arguments) == -1)
        {
            printf("Failed to exec.\n");
            exit(-1);
        }
	}
}

bool validate_resources(int available, int needed)
{
	if (available >= needed)
	{
		return true;
	}
	
	return false;
}

void start_scheduler()
{
	Process* current = NULL;
	
	incoming_process_list = malloc(sizeof(List));
	real_time_process_list = malloc(sizeof(List));
	user_high_process_list = malloc(sizeof(List));
	user_normal_process_list = malloc(sizeof(List));
	user_low_process_list = malloc(sizeof(List));
	user_wait_process_list = malloc(sizeof(List));
	
	while (true)
	{
		if (!empty(incoming_process_list)) // Check for incoming processes.
		{
			dispatch_process((Process*)(front(incoming_process_list))->value);
			pop_front(incoming_process_list);
		}
		
		if (!empty(real_time_process_list)) // Check if there is any real-time processes.
		{
			if (current->priority != REAL_TIME && current->running)
			{
				// TODO Pause user process if one is currently running.
			}
			
			current = (Process*)(front(real_time_process_list))->value;
			
			if (!current->running)
			{
				execute_process(current);
				// TODO Print process information.
				current->running = true;
			}
			
			else if (current->remaining_time <= 0 && current->running)
			{
				kill(current->pid, SIGINT);
				delete_process(real_time_process_list, current);
				current->running = false;
			}
		}
		
		else // No real-time processes available, check for user processes.
		{
			// TODO
		}
		
		if (current->is_running)
		{
			--current->remaining_time;
		}
		
		sleep(QUANTUM);
	}
	
	free(incoming_process_list);
	free(real_time_process_list);
	free(user_high_process_list);
	free(user_normal_process_list);
	free(user_low_process_list);
	free(user_wait_process_list);
}
