#ifndef __LOG_H__
#define __LOG_H__

#define LOGGING_ON
#define LOGINST													__FILE__, __LINE__

#define __LOG_OFF  										    	0UL
#define __LOG_EMERG  										    5UL
#define __LOG_CRIT	        								    10UL
#define __LOG_ERROR											    100UL
#define __LOG_WARN											    500UL
#define __LOG_MSG											    1000UL
#define __LOG_TRACE											    2000UL
#define __LOG_INFO  										    4000UL
#define __LOG_NOT	  										    5000UL

//
// standard logging levels
//
#define LOG_EMERGENCY											LOGINST, __LOG_EMERG
#define LOG_CRITICAL    										LOGINST, __LOG_CRIT
#define LOG_ERROR			    								LOGINST, __LOG_ERROR
#define LOG_WARN												LOGINST, __LOG_WARN
#define LOG_MSG	    											LOGINST, __LOG_MSG
#define LOG_DEBUG    											LOGINST, __LOG_MSG
#define LOG_TRACE	    										LOGINST, __LOG_TRACE
#define LOG_INFO												LOGINST, __LOG_INFO
#define LOG_NOT													LOGINST, __LOG_NOT

//
// function prototypes & externs
//

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#ifdef LOGGING_ON

	unsigned long log_open(const char *file, const unsigned long linenum, const unsigned long level,
									const char *logfilename);

	unsigned long log_write(const char *file, const unsigned long linenum, const unsigned long level, 
									const char *fmt, ...);

	unsigned long log_set_level(const char *file, const unsigned long linenum,
									unsigned long new_logging_level);

	unsigned long log_close(const char *file, const unsigned long linenum, const unsigned long level);

#else
	_inline unsigned long log_open(const char *file, const unsigned long linenum, const unsigned long level,
									const char *logfilename) {}
	_inline unsigned long log_write(const char *file, const unsigned long linenum, 
									const unsigned long level, const char *fmt, ...) {}
	_inline unsigned long log_set_level(const char *file, const long linenum, 
									unsigned long new_logging_level) {}
	_inline unsigned long log_close(const char *file, const unsigned long linenum,
									const unsigned long level) {}
#endif


#ifdef __cplusplus
           }
#endif //__cplusplus

#endif //__LOG_H__
