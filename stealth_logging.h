/**
 * Copyright Stealth Software Technologies, Inc.
 */

/* This line tells Mingw not to use printf from MSVCRT, which only conforms
 * to C89, and can't do things like %zx to print a size_t or pointer value */
#ifdef _WIN32
#ifndef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 1
#endif // __USE_MINGW_ANSI_STDIO
#endif // _WIN32

#include <cstdlib>
#include <cstdio>
#include <cerrno>

// decide whether to use datetime or not (specifics are handled in cpp)
#if defined(LOG_OPT_DATETIME) \
  || defined(LOG_OPT_DATE) \
  || defined(LOG_OPT_TIME)
#define LOG_USE_DATETIME
#endif

#ifndef LOGGING_H_
#define LOGGING_H_
// global variables which are guarded by the file, (and by various options)

extern FILE* LOG_FILE;

#ifdef LOG_OPT_COLOR_COND
extern bool LOG_COLORIZE;
#endif

#ifndef LOG_OPT_IDENTIFIER
extern thread_local char const* LOG_IDENTIFIER_STR;
void log_set_identifier(char const*);
char const* log_get_identifier();
#endif

#ifdef LOG_USE_DATETIME
char const* log_datetime_string();
#endif

char const* log_perror_string();

#ifdef _WIN32
char const* log_wperror_string();
#endif

void log_stack_trace();

#endif//LOGGING_H_

// Most of the macro code is guarded by the "UNINCLUDE"
#ifndef LOG_UNINCLUDE

// convert a something to a string literal
#define LOG_STRINGIFY_HELPER(...) #__VA_ARGS__
#define LOG_STRINGIFY(...) LOG_STRINGIFY_HELPER(__VA_ARGS__)

// Color definitions
#if defined(LOG_OPT_COLOR_COND) || !defined(LOG_OPT_COLOR_NONE)
#define LOG_COLOR_RED "\033[0;31m"
#define LOG_COLOR_RED_INVERT "\033[0;41m"
#define LOG_COLOR_GREEN "\033[0;32m"
#define LOG_COLOR_GREEN_INVERT "\033[0;42m"
#define LOG_COLOR_YELLOW "\033[0;33m"
#define LOG_COLOR_BLUE "\033[0;34m"
#define LOG_COLOR_MAGENTA "\033[0;35m"
#define LOG_COLOR_CYAN "\033[0;36m"
#define LOG_COLOR_DEFAULT "\033[0m"
#else
#define LOG_COLOR_RED ""
#define LOG_COLOR_RED_INVERT ""
#define LOG_COLOR_GREEN ""
#define LOG_COLOR_GREEN_INVERT ""
#define LOG_COLOR_YELLOW ""
#define LOG_COLOR_BLUE ""
#define LOG_COLOR_MAGENTA ""
#define LOG_COLOR_CYAN ""
#define LOG_COLOR_DEFAULT ""
#endif

// FMT is built into the format string, and VAL into the format arguments.
#ifdef LOG_OPT_COLOR_COND
#define LOG_COLOR_FMT(val) "%s"
#define LOG_COLOR_VAL(val) (LOG_COLORIZE) ? val : "",
#define LOG_UNCOLOR_FMT "%s"
#define LOG_UNCOLOR_VAL (LOG_COLORIZE) ? LOG_COLOR_DEFAULT : "",
#else
#define LOG_COLOR_FMT(val) val
#define LOG_COLOR_VAL(val)
#define LOG_UNCOLOR_FMT LOG_COLOR_DEFAULT
#define LOG_UNCOLOR_VAL
#endif

// Source file and line options. built into the format string.
#ifdef LOG_OPT_FILE_LINE
#define LOG_CURRENT_FILE_LINE " (" __FILE__ ":" LOG_STRINGIFY(__LINE__) ")"
#else
#define LOG_CURRENT_FILE_LINE
#endif

// logging identifer, FMT gets built into the format string and VAL gets
// built into the format arguments
#ifdef LOG_OPT_IDENTIFIER
#undef LOG_IDENTIFIER
#define LOG_IDENTIFIER LOG_STRINGIFY(LOG_OPT_IDENTIFIER)
#endif

#ifndef LOG_IDENTIFIER
#define LOG_IDENTIFIER_FMT "%s"
#define LOG_IDENTIFIER_VAL log_get_identifier(),
#else
#define LOG_IDENTIFIER_FMT LOG_IDENTIFIER
#define LOG_IDENTIFIER_VAL
#endif

// log date/time, FMT goes in the format string, VAL in the format args.
#ifdef LOG_USE_DATETIME
#define LOG_DATETIME_FMT "%s "
#define LOG_DATETIME_VAL log_datetime_string(),
#else
#define LOG_DATETIME_FMT ""
#define LOG_DATETIME_VAL
#endif

#define LOG_FUNC(level, color, fmt, ...) \
  do { \
    fprintf(LOG_FILE, \
        LOG_COLOR_FMT(color) \
        LOG_DATETIME_FMT \
        level \
        LOG_CURRENT_FILE_LINE \
        " (" \
        LOG_IDENTIFIER_FMT \
        "):" \
        LOG_UNCOLOR_FMT \
        " " \
        fmt \
        "%s\n", \
        LOG_COLOR_VAL(color) \
        LOG_DATETIME_VAL \
        LOG_IDENTIFIER_VAL \
        LOG_UNCOLOR_VAL \
        __VA_ARGS__); \
    fflush(LOG_FILE); \
  } while(0)

#if defined(LOG_ENABLE_TRACE) || defined(LOG_OPT_ENABLE_TRACE)
#define log_trace(...) LOG_FUNC("TRACE", LOG_COLOR_CYAN, __VA_ARGS__, "")
#else
#define log_trace(...)
#endif

#if defined(LOG_ENABLE_DEBUG) || defined(LOG_OPT_ENABLE_DEBUG)
#define log_debug(...) LOG_FUNC("DEBUG", LOG_COLOR_MAGENTA, __VA_ARGS__, "")
#else
#define log_debug(...)
#endif

#if defined(LOG_DISABLE_INFO) || defined(LOG_OPT_DISABLE_INFO)
#define log_info(...)
#else
#define log_info(...) LOG_FUNC(" INFO", LOG_COLOR_BLUE, __VA_ARGS__, "")
#endif

#if defined(LOG_DISABLE_WARN) || defined(LOG_OPT_DISABLE_WARN)
#define log_warn(...)
#else
#define log_warn(...) LOG_FUNC(" WARN", LOG_COLOR_YELLOW, __VA_ARGS__, "")
#endif

#if defined(LOG_DISABLE_ERROR) || defined(LOG_OPT_DISABLE_ERROR)
#define log_error(...)
#else
#define log_error(...) LOG_FUNC("ERROR", LOG_COLOR_RED, __VA_ARGS__, "")
#endif

#ifdef _WIN32
#define log_wperror() log_error("%s", log_wperror_string())
#endif

#define log_perror() log_error("%s", log_perror_string())

#define log_fatal(...) \
  do { \
    LOG_FUNC("FATAL", LOG_COLOR_RED_INVERT, __VA_ARGS__, ""); \
    log_stack_trace(); \
    abort(); \
  } while(0)

#ifndef NDEBUG
#define log_assert(cond, ...) \
  do { \
    if(!(cond)) { \
      log_fatal("Assertion Failure (" LOG_STRINGIFY(cond) ") " __VA_ARGS__); \
    } \
  } while (0)
#else
#define log_assert(cond, ...)
#endif

#else // LOG_UNINCLUDE

#undef LOG_USE_DATETIME

#undef LOG_STRINGIFY_HELPER
#undef LOG_STRINGIFY

#undef LOG_COLOR_RED
#undef LOG_COLOR_RED_INVERT
#undef LOG_COLOR_GREEN
#undef LOG_COLOR_GREEN_INVERT
#undef LOG_COLOR_YELLOW
#undef LOG_COLOR_BLUE
#undef LOG_COLOR_MAGENTA
#undef LOG_COLOR_CYAN
#undef LOG_COLOR_DEFAULT

#undef LOG_COLOR_FMT
#undef LOG_COLOR_VAL
#undef LOG_UNCLOR_FMT
#undef LOG_UNCOLOR_VAL

#undef LOG_CURRENT_FILE_LINE

#undef LOG_IDENTIFIER
#undef LOG_IDENTIFIER_FMT
#undef LOG_IDENTIFIER_VAL

#undef LOG_DATETIME_FMT
#undef LOG_DATETIME_VAL

#undef LOG_FUNC

#undef LOG_ENABLE_TRACE
#undef log_trace

#undef LOG_ENABLE_DEBUG
#undef log_debug

#undef LOG_DISABLE_INFO
#undef log_info

#undef LOG_DISABLE_WARN
#undef log_warn

#undef LOG_DISABLE_ERROR
#undef log_error

#undef log_wperror
#undef log_perror

#undef log_fatal
#undef log_assert

#undef LOG_UNINCLUDE

#endif // LOG_UNINCLUDE
