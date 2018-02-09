#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "scheduler.h"

#define DELEMITER ","

void fill_process_list(char* filename) {
	FILE* fp = fopen(filename, "r");
	char* line = NULL;
	char* str_val = NULL;
	int i_val[7];
	int i;
	size_t line_len;

	if(fp == NULL)
	{
		printf("Failed to open %s!\n%s\n", filename, strerror(errno));
		exit(1);
	}
	else
	{
		while(getline(&line, &line_len, fp) != -1) {
			i = 0;
			str_val = strtok(line, DELEMITER);
			while(str_val)
			{
				if(i > 7)
				{
					break;
				}
				i_val[i++] = (int)strtol(str_val, NULL, 10);
				str_val = strtok(NULL, DELEMITER);
			}

			if(i != 7)
			{
				printf("File %s is malformed!\n", filename);
				exit(1);
			}
			else
			{
				create_process(i_val[0], i_val[1], i_val[2], i_val[3], i_val[4], i_val[5], i_val[6]);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc < 2) {
		printf("USAGE: %s [process-list-file]\n\n", argv[0]);
		return 1;
	}
	else {
		initialize_scheduler();
		fill_process_list(argv[1]);
		start_scheduler();
	}
    return 0;
}