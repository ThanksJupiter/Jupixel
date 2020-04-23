#include "Application.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	bool success = init_application();

	if (!success)
	{
		printf("Unable to initialize :(");
		return -1;
	}

	run_application();
	quit_application();

	return 0;
}
