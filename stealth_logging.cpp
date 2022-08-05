/**
 * Copyright Stealth Software Technologies, Inc.
 */

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cerrno>

#ifdef __GLIBC__
#include <execinfo.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

FILE* LOG_FILE = stderr;

#ifdef LOG_OPT_COLOR_COND
bool LOG_COLORIZE = true;
#endif

#ifndef LOG_OPT_IDENTIFIER
thread_local char const* LOG_IDENTIFIER_STR = "main";

void log_set_identifier(char const* id)
{
  LOG_IDENTIFIER_STR = id;
}
char const* log_get_identifier()
{
  return LOG_IDENTIFIER_STR;
}
#endif

#if defined(LOG_OPT_DATETIME) \
  || (defined(LOG_OPT_DATE) && defined(LOG_OPT_TIME))
#define LOG_USE_DATETIME
#define DATETIME_FMT "%Y-%m-%d %H:%M:%S"
#elif defined(LOG_OPT_DATE) 
#define LOG_USE_DATETIME
#define DATETIME_FMT "%Y-%m-%d"
#elif defined(LOG_OPT_TIME)
#define LOG_USE_DATETIME
#define DATETIME_FMT "%H:%M:%S"
#endif

#ifdef LOG_USE_DATETIME

size_t const LOG_DATETIME_STR_LEN = 32;
thread_local char log_datetime_str[LOG_DATETIME_STR_LEN + 1];

#ifdef _WIN32
#define localtime_r(t, r) localtime_s((r), (t))
#endif

char const* log_datetime_string()
{
  time_t now;
  tm time_obj;
  time(&now);
  localtime_r(&now, &time_obj);
  memset((void*) log_datetime_str, 0, LOG_DATETIME_STR_LEN + 1);
  strftime(log_datetime_str, LOG_DATETIME_STR_LEN, DATETIME_FMT, &time_obj);
  return log_datetime_str;
}

#endif

size_t const LOG_ERROR_STR_LEN = 512;
thread_local char log_error_str[LOG_ERROR_STR_LEN + 1];

char const* log_perror_string()
{
  int err_num = errno;
  memset((void*) log_error_str, 0, LOG_ERROR_STR_LEN + 1);
#if _WIN32
  strerror_s(log_error_str, LOG_ERROR_STR_LEN, err_num);
  return log_error_str;
#elif ((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE) \
  || __APPLE__
  strerror_r(err_num, log_error_str, LOG_ERROR_STR_LEN);
  return log_error_str;
#else
  // dammnit GNU
  char const* err_str = strerror_r(err_num, log_error_str, LOG_ERROR_STR_LEN);
  return err_str;
#endif
}

#ifdef _WIN32
char const* log_wperror_string()
{
  memset((void*) log_error_str, 0, LOG_ERROR_STR_LEN + 1);
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0,
      log_error_str, LOG_ERROR_STR_LEN, NULL);
  return log_error_str;
}
#endif

void log_stack_trace()
{
#ifdef __GLIBC__
  fprintf(LOG_FILE, "backtrace:\n");
  void * backtraces[256];
  int num_backtraces = backtrace(backtraces, 256);
  backtrace_symbols_fd(backtraces, num_backtraces, fileno(LOG_FILE));
  fflush(LOG_FILE);
#endif //__GLIBC__
}
