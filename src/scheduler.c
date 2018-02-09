#include "scheduler.h"
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

void dispatch_process(Process* process)
{
	switch (process->priority)
	{
		case REAL_TIME:
			list_push_back(real_time_process_list, process);
			break;
		default:
			switch(process->priority) // TODO Dispatch accordingly [list_push_back / list_push_front / insert].
			{
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
			break;
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

void manage_process(List* list, Process** process, Priority priority)
{
	Node* node = NULL;
	
	if (*process && (*process)->priority < priority && (*process)->is_running)
	{
		kill((*process)->pid, SIGSTOP);
		// TODO waitpid
		
		if ((*process)->priority > USER_LOW && priority < REAL_TIME)
		{
			--(*process)->priority;
		}
	}
	
	node = list_front(list);
	*process = (Process*)node->value;
	
	if (*process)
	{
		if (!(*process)->is_running)
		{
			execute_process(*process);
			print_process(*process);
			(*process)->is_running = true;
		}
	
		else if ((*process)->remaining_time <= 0 && (*process)->is_running)
		{
			kill((*process)->pid, SIGINT);
			free(*process);
			*process = NULL;
			list_remove(list, node);
		}
	}
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
	Process* process = NULL;
	
	while (true)
	{
		printf("%d %d %d %d %d\n", incoming_process_list->size, real_time_process_list->size, user_high_process_list->size, user_normal_process_list->size, user_low_process_list->size);
		
		if (!list_empty(incoming_process_list)) // Check for incoming processes.
		{
			dispatch_process((Process*)(list_front(incoming_process_list))->value);
			list_pop_front(incoming_process_list);
		}
		
		if (!list_empty(real_time_process_list)) // Check if there is any real-time processes.
		{
			manage_process(real_time_process_list, &process, REAL_TIME);
		}
		
		else // No real-time processes available, check for user processes.
		{
			if (!list_empty(user_high_process_list)) // Check if there is any high priority processes.
			{	
				manage_process(user_high_process_list, &process, USER_HIGH);
			}
			
			else if (!list_empty(user_normal_process_list)) // Check if there is any normal priority processes.
			{
				manage_process(user_normal_process_list, &process, USER_NORMAL);
			}
			
			else if (!list_empty(user_low_process_list)) // Check if there is any low priority processes.
			{
				manage_process(user_low_process_list, &process, USER_LOW);
			}
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
