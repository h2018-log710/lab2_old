#include "scheduler.h"

int main(int argc, char* argv[])
{
	// TODO Parse text file.
	
	initialize_scheduler();
	create_process(12, 0, 1, 0, 0, 0, 0);
	start_scheduler();
	
    return 0;
}
