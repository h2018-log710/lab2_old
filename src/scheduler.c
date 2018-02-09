#include "scheduler.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	process->state = NEW;
	process->arrival_time = arrival_time;
	process->execution_time = execution_time;
	process->remaining_time = execution_time;
	process->printers = printers;
	process->scanners = scanners;
	process->modems = modems;
	process->cds = cds;
	
	list_push_back(incoming_process_list, process);
}

void dispatch_process(int tick)
{
	Node* node = incoming_process_list->head;
	Process* process = NULL;

	while (node)
	{
		process = (Process*)node->value;
		
		if (process->arrival_time == tick)
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
			
			process->state = READY;
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

void manage_process(List* list, Node** node, Priority priority)
{
	Process* process = NULL;
	
	if (*node)
	{
		process = (Process*)(*node)->value;
	}
	
	if (process)
		printf("List Priority: %d Priority: %d State: %d.\n", priority, process->priority, process->state);
	
	// Check if the current process must be paused.
	if (process && priority < process->priority && process->state == RUNNING)
	{
		kill(process->pid, SIGTSTP);
		waitpid(process->pid, NULL, WUNTRACED);
		process->state = WAITING;
		
		// Check if the priority of the current process is at least higher than low.
		if (process->priority < USER_LOW)
		{
			/*++process->priority; // TODO Increment or decrement?
			
			switch (process->priority)
			{
				case USER_NORMAL:
					printf("List Size Before: %d ", list->size);
					list_remove(user_high_process_list, *node);
					
					list_push_back(user_normal_process_list, *node);
					printf("List Size After: %d\n", list->size);
					break;
				case USER_LOW:
					list_remove(user_normal_process_list, *node);
					list_push_back(user_low_process_list, *node);
					break;
				default:
					printf("Invalid process change.\n");
					exit(-1);
					break;
			}*/
		}
	}
	
	*node = list_front(list);
	process = (Process*)(*node)->value;
	
	if (process)
	{
		// Check if the current process is ready to be run.
		if (process->state == READY)
		{
			execute_process(process);
			print_process(process);
			process->state = RUNNING;
		}
		
		// Check if the current process is paused.
		else if (process->state == WAITING)
		{
			kill(process->pid, SIGCONT);
			process->state = RUNNING;
		}
		
		// Check if the current process is running.
		else if (process->state == RUNNING)
		{
			--process->remaining_time;
			
			// Check if the current process is ready to be terminated.
			if (process->remaining_time <= 0)
			{
				kill(process->pid, SIGINT);
				waitpid(process->pid, NULL, 0);
			
				free(process);
				(*node)->value = NULL;
				list_remove(list, *node);
			}
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
	Node* node = NULL;
	int tick = 0;
	
	while (incoming_process_list->size != 0
	    	|| real_time_process_list->size != 0
		|| user_high_process_list->size != 0
		|| user_normal_process_list->size != 0
		|| user_low_process_list->size != 0
		|| user_wait_process_list->size != 0)
	{
		++tick;
		printf("Tick: %d ", tick);
		printf("Lists: %d %d %d %d %d %d\n", incoming_process_list->size, real_time_process_list->size, user_high_process_list->size,
			user_normal_process_list->size, user_low_process_list->size, user_wait_process_list->size);
		
		// Check for incoming processes.
		if (!list_empty(incoming_process_list))
		{
			dispatch_process(tick);
		}
		
		// Check if there is any real-time processes.
		if (!list_empty(real_time_process_list))
		{
			manage_process(real_time_process_list, &node, REAL_TIME);
		}
		
		// No real-time processes available, check for user processes.
		else
		{
			// Check if there is any high priority processes.
			if (!list_empty(user_high_process_list))
			{	
				manage_process(user_high_process_list, &node, USER_HIGH);
			}
			
			// Check if there is any normal priority processes.
			else if (!list_empty(user_normal_process_list))
			{
				manage_process(user_normal_process_list, &node, USER_NORMAL);
			}
			
			// Check if there is any low priority processes.
			else if (!list_empty(user_low_process_list))
			{
				manage_process(user_low_process_list, &node, USER_LOW);
			}
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
