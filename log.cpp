#include <stdarg.h>
#include <stdio.h>
#include <string>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#define PATH_SEP_CHAR '\\'
#else
#include <sys/timeb.h>
#define PATH_SEP_CHAR '/'
#endif

#include <limits.h>
#include "log.h"

#define LOG_FMT_STR "%010lu|%s|%lu| %s\n"

static FILE 		   *gLogFile = NULL;
static unsigned long    gLogStartTick = 0;
static unsigned long	gLogLevel = LONG_MIN;

#ifdef LOGGING_ON

unsigned long
GetLogTickCount()
{
	unsigned long log_time = 0;

#ifndef WIN32
	timeb timeNow;
	ftime(&timeNow);
	log_time = timeNow.time * 100;
	log_time += timeNow.millitm;
	//return timeNow.time * 1000 + timeNow.millitm;
#else
    log_time =  GetTickCount();
#endif

	return log_time;
}

unsigned long
log_open(const char *file, const unsigned long linenum, const unsigned long default_level, 
		 const char *file_name)
{
	gLogLevel = default_level;

	if (gLogFile)
	{
		log_write(file, linenum, __LOG_ERROR, "Attempt to re-open log file.");
		return 10;
	}

	if (NULL == file_name || '\0' == *file_name)
		return 20;

	gLogStartTick = GetLogTickCount();

	if (file_name != NULL)
	{
#ifdef WIN32
		errno_t ret;
		ret = fopen_s(&gLogFile, file_name, "w");
		if (0 != ret)
			return 30;
#else
		gLogFile = fopen(file_name, "w");
#endif
		if (gLogFile == NULL)
			return 40;
	}

	log_write(file, linenum, __LOG_TRACE, "Log file opened.");
	return 0;
}

unsigned long
log_close(const char *file, const unsigned long linenum, const unsigned long level)
{
	if (!gLogFile)
	{
		if (level <= gLogLevel)
			log_write(file, linenum, __LOG_ERROR, "Attempt to close unopened log file.");
		return 10;
	}

	if (level <= gLogLevel)
		log_write(file, linenum, __LOG_TRACE, "Closing log file.");
	fclose(gLogFile);
	gLogFile = NULL;
	return 0;
}

unsigned long
log_set_level(const char *file, const unsigned long linenum, 
				const unsigned long new_logging_level)
{
	log_write(file, linenum, __LOG_TRACE, 
			"Changing log level from %lu to %lu.",
			gLogLevel, new_logging_level);
	gLogLevel = new_logging_level;
	return 0;
}

unsigned long
log_write(const char *file, const unsigned long linenum, const unsigned long level, const char *fmt,...)
{
	va_list	args;
	char    str[2048];
	char    log_msg_buf[4096];

	if (gLogFile == NULL)
		return 10;

	if (level > gLogLevel)
		return 0;

	std::string FileName(file);
	std::string LocalFileName(file);

	std::string::size_type pos, len;
	pos = LocalFileName.find_last_of(PATH_SEP_CHAR, FileName.size());
	len = LocalFileName.size();

	if (std::string::npos != pos && pos < len)
		LocalFileName = LocalFileName.substr(pos+1, std::string::npos);

	if (0 == LocalFileName.size())
	{
		LocalFileName = "NoFileName";
	}

	va_start(args, fmt);
#ifdef WIN32
	vsprintf_s(str, sizeof str, fmt, args);
#else
	vsnprintf(str, sizeof str, fmt, args);
#endif
	va_end(args);

	//swprintf_s(log_msg_buf_w, sizeof log_msg_buf_w, L"%s", str);

#ifdef WIN32
	sprintf_s(log_msg_buf, sizeof log_msg_buf,
			LOG_FMT_STR,
			GetLogTickCount() - gLogStartTick,
			(LocalFileName.length() ? LocalFileName.c_str() : "NoName"), 
			linenum,
			str);

	#ifdef _DEBUG
		OutputDebugString(log_msg_buf);
	#endif
#else
	snprintf(log_msg_buf, sizeof log_msg_buf, 
			LOG_FMT_STR,
			GetLogTickCount() - gLogStartTick,
			(LocalFileName.length() ? LocalFileName.c_str() : "NoName"), 
			linenum,
			str);
#endif

#ifdef WIN32
	fprintf_s(gLogFile, "%s", log_msg_buf);
#else
	fprintf(gLogFile, "%s", log_msg_buf);
#endif

	fflush(gLogFile);
	return 0;
}

#endif
