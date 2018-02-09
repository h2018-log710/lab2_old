#include "scheduler.h"
#include "list.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static const int QUANTUM = 1;

static List* incoming_process_list = NULL;	// Incoming processes yet to be dispatched.
static List* real_time_process_list = NULL;	// Real-time processes ready to be executed.
static List* user_high_process_list = NULL;	// User processes with high priority ready to be executed.
static List* user_normal_process_list = NULL;	// User processes with normal priority ready to be executed.
static List* user_low_process_list = NULL;	// User processes with low priority ready to be executed.
static List* user_wait_process_list = NULL;	// User processes waiting for resources to be available.

void create_process(int arrival_time, Priority priority, int execution_time, int printers, int scanners, int modems, int cds)
{
	Process* process = malloc(sizeof(Process));
	
	process->pid = 0;
	process->priority = priority;
	process->arrival_time = arrival_time;
	process->execution_time = execution_time;
	process->remaining_time = execution_time;
	process->printers = printers;
	process->scanners = scanners;
	process->modems = modems;
	process->cds = cds;
	process->is_paused = false;
	process->is_running = false;
	
	list_push_back(incoming_process_list, process);
}

void dispatch_process(int tick)
{
	Node* node = incoming_process_list->head;
	Process* process = NULL;

	while(node)
	{
		process = (Process*)node->value;
		if(process->arrival_time == tick)
		{
			switch (process->priority)
			{
				case REAL_TIME:
					list_push_back(real_time_process_list, process);
					break;
				case USER_HIGH:
					list_push_back(user_high_process_list, process);
					break;
				case USER_NORMAL:
					list_push_back(user_normal_process_list, process);
					break;
				case USER_LOW:
					list_push_back(user_low_process_list, process);
					break;
				default:
					printf("Invalid process priority.\n");
					exit(-1);
					break;
			}
			list_remove(incoming_process_list, node);
		}
		node = node->next;
	}
}

void execute_process(Process* process)
{
	pid_t pid = fork();
	
	if (pid == -1)
    	{
        	printf("Failed to fork.\n");
        	exit(-1);
    	}
    
    	else if (pid == 0)
    	{
		char* arguments[2];
		arguments[0] = "./bin/log710h15process";
		arguments[1] = NULL;
		
		if (execvp(arguments[0], arguments) == -1)
		{
		    printf("Failed to exec.\n");
		    exit(-1);
		}
	}
	
	else
	{
		process->pid = pid;
	}
}

void print_process(Process* process)
{
	printf("PID: %d, Arrival Time: %d, Priority: %d, Remaining Execution Time: %d, Printers: %d, Scanners: %d, Modems: %d, CDs: %d\n",
		process->pid, process->arrival_time, process->priority, process->remaining_time, process->printers, process->scanners, process->modems, process->cds);
}

bool validate_resources(int available, int needed)
{
	if (available >= needed)
	{
		return true;
	}
	
	return false;
}

void initialize_scheduler()
{
	incoming_process_list = malloc(sizeof(List));
	real_time_process_list = malloc(sizeof(List));
	user_high_process_list = malloc(sizeof(List));
	user_normal_process_list = malloc(sizeof(List));
	user_low_process_list = malloc(sizeof(List));
	user_wait_process_list = malloc(sizeof(List));
}

void start_scheduler()
{
	Node* node = NULL;
	Process* process = NULL;
	int tick = 0;
	
	while (incoming_process_list->size != 0
	    || real_time_process_list->size != 0
		|| user_high_process_list->size != 0
		|| user_normal_process_list->size != 0
		|| user_low_process_list->size != 0
		|| user_wait_process_list->size != 0)
	{
		tick++;
		printf("Tick: %d\n", tick);

		if (!list_empty(incoming_process_list)) // Check for incoming processes.
		{
			dispatch_process(tick);
		}
		
		if (!list_empty(real_time_process_list)) // Check if there is any real-time processes.
		{
			if (process && process->priority != REAL_TIME && process->is_running)
			{
				// TODO Pause user process if one is currently running.
			}
			
			node = list_front(real_time_process_list);
			process = (Process*)node->value;
			
			if (!process->is_running)
			{
				execute_process(process);
				print_process(process);
				process->is_running = true;
			}
			
			else if (process->remaining_time <= 0 && process->is_running)
			{
				kill(process->pid, SIGINT);
				free(process);
				list_remove(real_time_process_list, node);
			}
		}
		
		else // No real-time processes available, check for user processes.
		{
			// TODO
		}
		
		if (process && process->is_running)
		{
			--process->remaining_time;
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
