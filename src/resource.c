#include "resource.h"
#include "scheduler.h"

Resource printer_list[TOTAL_PRINTER_COUNT];
Resource scanner_list[TOTAL_SCANNER_COUNT];
Resource modem_list[TOTAL_MODEM_COUNT];
Resource cd_list[TOTAL_CD_COUNT];

int printer_available = TOTAL_PRINTER_COUNT;
int scanner_available = TOTAL_SCANNER_COUNT;
int modem_available = TOTAL_MODEM_COUNT;
int cd_available = TOTAL_CD_COUNT;

void __do_indv_assign_resource(Process* process, Resource resource_list[], int count, int total_count)
{
    Resource* resource;

    for(int i = 0; i < total_count; i++)
    {
        if(count == 0)
        {
            break;
        }

        resource = &resource_list[i];
        if(resource->assignedProcess == NULL)
        {
            resource->assignedProcess = process;
            --count;
        }
    }
}

int __do_indv_free_resource(Process* process, Resource resource_list[], int total_count)
{
    Resource* resource;
    int count = 0;

    for(int i = 0; i < total_count; i++)
    {
        resource = &resource_list[i];
        if(resource->assignedProcess == process)
        {
            resource->assignedProcess = NULL;
            count++;
        }
    }

    return count;
}

/**
 * Assign resources to a process
 * Return 1 if the requested resource are not available
 * Return 0 if resources were successfully assigned
 */
int assign_resources(Process* process, int printer_count, int scanner_count, int modem_count, int cd_count)
{
    if(printer_count > printer_available
    || scanner_count > scanner_available
    || modem_count   > modem_available
    || cd_count      > cd_available)
    {
        return 1;
    }
    else
    {
        __do_indv_assign_resource(process, printer_list, printer_count, TOTAL_PRINTER_COUNT);
        __do_indv_assign_resource(process, scanner_list, scanner_count, TOTAL_SCANNER_COUNT);
        __do_indv_assign_resource(process, modem_list, modem_count, TOTAL_MODEM_COUNT);
        __do_indv_assign_resource(process, cd_list, cd_count, TOTAL_CD_COUNT);

        printer_available -= printer_count;
        scanner_available -= scanner_count;
        modem_available   -= modem_count;
        cd_available      -= cd_count;
    }

    return 0;
}

/**
 * Free resources used up by a process
 */
void free_resource(Process *process)
{
    printer_available += __do_indv_free_resource(process, printer_list, TOTAL_PRINTER_COUNT);
    scanner_available += __do_indv_free_resource(process, scanner_list, TOTAL_SCANNER_COUNT);
    modem_available   += __do_indv_free_resource(process, modem_list, TOTAL_MODEM_COUNT);
    cd_available      += __do_indv_free_resource(process, cd_list, TOTAL_CD_COUNT);
}