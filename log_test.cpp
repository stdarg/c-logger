#include <cstdio>
#include "log.h"

#define FILE_NAME "./test.log"

int
create_file()
{
	unsigned long ret;

	ret = log_open(LOG_TRACE, FILE_NAME);
	if (0 != ret)
		return 100;

	ret = log_close(LOG_TRACE);
	if (0 != ret)
		return 200;

	return 0;

}

int main(int argc, char *argv[])
{
	unsigned long ret;

	ret = create_file();
	if (ret != 0)
	{
		fprintf(stderr, "There was a problem writing to the log file.\n");
	}

	return 0;
}
