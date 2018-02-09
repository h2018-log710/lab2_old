#ifndef RESOURCE_H
#define RESOURCE_H

#include "scheduler.h"

#define TOTAL_PRINTER_COUNT 2
#define TOTAL_SCANNER_COUNT 1
#define TOTAL_MODEM_COUNT   1
#define TOTAL_CD_COUNT      2

typedef struct Resource
{
    Process* assignedProcess;
} Resource;

int assign_resource(Process* process, int printer_count, int scanner_count, int modem_count, int cd_count);
void free_resource(Process* process);

#endif