/**
 * @file log.h
 * @brief Contains functions for logging engine messages.
 * @if doc_files
 * @ingroup smug_common
 * @endif
 */

/**
 * @addtogroup smug_common
 * @{
 */

#ifndef SMUG_COMMON_LOG_H
#define SMUG_COMMON_LOG_H

#include <stdarg.h>

#include <common.h>
#include <utils/console.h>

// Predefined log levels
#define LOG_DEBUG           0x01
#define LOG_NOTIFICATION    0x02
#define LOG_WARNING         0x04
#define LOG_ERROR           0x08
#define LOG_ALL             0xFF
#define LOG_NONE            0x00

// Define macros for the Log_Write function
#define DEBUG(...) Log_addEntry(LOG_DEBUG, __FILE__, __LINE__, ##__VA_ARGS__)
#define NOTIFY(...) Log_addEntry(LOG_NOTIFICATION, __FILE__, __LINE__, ##__VA_ARGS__)
#define WARNING(...) Log_addEntry(LOG_WARNING, __FILE__, __LINE__, ##__VA_ARGS__)
#define ERROR(...) Log_addEntry(LOG_ERROR, __FILE__, __LINE__, ##__VA_ARGS__)

/** Initialize the log system (allocate memory, etc)
 */
BOOL Log_init(Console* console);


/** Returns true if the system is initialized
 */
BOOL Log_isInitialized(void);


/** Terminate the log system (free memory, etc)
 */
void Log_terminate(void);


/** Write a log entry.
  *
  * Use the macros DEBUG(char*, ...), WARNING(char*, ...), ERROR(char*, ...), etc instead.
  */
void Log_addEntry(int level, char* file, int line, char* fmt, ...);

void Log_addEntryVa(int level, char* file, int line, char* fmt, va_list args);


/** Set the log level to be written by Log_Write.
  *
  * @param level A flagset consisting of LOG_DEBUG, LOG_WARNING and/or LOG_ERROR
  */
void Log_setLevel(int level);


/** Get the curent log level.
  *
  * @return A flagset consisting of the log level constants.
  */
int Log_getLevel(void);


/** Set the format string to use for log output
  *
  * The following flags are recognized:
  *
  * %file%     -  The name of the file where the log command was issued
  * %line%     -  The line number in above file
  * %scope%    -  The log scope string, i.e. DEBUG, NOTICE, WARNING, etc
  * %indent%   -  Indentation will be applied here
  * %message%  -  The message printed by the user
  */
void Log_setFormatString(char* format_string);


/** Indent the log messages
 *
 * Make all following log message be indented an extra level
 */
void Log_indent(void);


/** Dedent the log messages
 *
 * Make all following log message be indented one level less
 */
void Log_dedent(void);


/** Set the tab length of the indentation
 *
 * Set how many spaces one indentation level should have
 */
void Log_setIndentationString(char* indentString);


#endif // SMUG_COMMON_LOG_H

/**@}*/
