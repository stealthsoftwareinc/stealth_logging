# Logging Tools

## Logging
The `stealth_logging.h` defines some logging functions
It is based on C-Macros expanding to ``printf`` statements.
All logging macros should be considered thread-safe (as printf itself is thread-safe).
the following function-like macros are defined:
 - ``log_trace(FMT, ...)``: produces log-level "trace", cyan in color, and default disabled.
 - ``log_debug(FMT, ...)``: produces log-level "debug", magenta in color, and default disabled.
 - ``log_info(FMT, ...)``: produces log-level "information", blue in color, and default enabled.
 - ``log_warn(FMT, ...)``: produces log-level "warning", yellow in color, and default enabled.
 - ``log_error(FMT, ...)``: produces log-level "error", red in color, and default enabled.

The following additional macros are defined.
 - ``log_perror()``: prints an error-level message with ``perror()`` message.
 - ``log_wperror()``: prints an error-level message with message coming from the Windows API ``GetLastError()`` syscall. Only available on windows.
 - ``log_fatal(FMT, ...)``: produces log-level "fatal", invoking ``abort()``, inverted-red in color, and cannot be disabled.
 - ``log_assert(COND, FMT, ...)``: when COND is false, it produces a ``log_fatal(...)`` message. Enabled only in "debug mode" (when ``NDEBUG`` is not defined).

The following preprocessor macros control logging. There are two groups of macros ``LOG_OPT_`` macros are intended for use as compiler flag options:
 - ``LOG_OPT_COLOR_NONE``: disables colors in logging (color is enabled by default).
 - ``LOG_OPT_COLOR_COND``: colors in logging are controlled by runtime boolean ``LOG_COLOR``.
 - ``LOG_OPT_FILE_LINE``: indicate the file and line-number of a log message.
 - ``LOG_OPT_IDENTIFIER``: Set an identifier to be used in log-messages globally.
 - ``LOG_OPT_TIME``: Print the time in ``%H:%M:%S`` format.
 - ``LOG_OPT_DATE``: Print the date in ``%Y-%m-%d`` format.
 - ``LOG_OPT_DATE_TIME``: alias for both ``LOG_OPT_TIME`` and ``LOG_OPT_DATE``.
 - ``LOG_OPT_ENABLE_TRACE``: enable all trace-level logging.
 - ``LOG_OPT_ENABLE_DEBUG``: enable all debug-level logging.
 - ``LOG_OPT_DISABLE_INFO``: disable all info-level logging.
 - ``LOG_OPT_DISABLE_WARN``: disable all warn-level logging.
 - ``LOG_OPT_DISABLE_ERROR``: disable all error-level logging.

The second group of macros are intended for per-file definition.
 - ``LOG_IDENTIFIER``: Set an identifier to be used in log-messages on a per-file basis.
 - ``LOG_ENABLE_TRACE``: enable trace-level logging on a per-file basis.
 - ``LOG_ENABLE_DEBUG``: enable debug-level logging on a per-file basis.
 - ``LOG_DISABLE_INFO``: disable info-level logging on a per-file basis.
 - ``LOG_DISABLE_WARN``: disable warn-level logging on a per-file basis.
 - ``LOG_DISABLE_ERROR``: disable error-level logging on a per-file basis.

The following global variables are used for runtime control over logging.
 - ``bool LOG_COLORIZE``: controls log colorizing when ``LOG_OPT_COLOR_COND`` is defined.
 - ``char const* LOG_IDENTIFIER_STR``: controls the identifier used in log-messages, when neither the ``LOG_OPT_IDENTIFIER`` nor ``LOG_IDENTIFIER`` **macros** are present. This variable is ``thread_local``.
   - To avoid thread-local-storage linking errors on Windows access this variable through ``void log_set_identifier(char const*)`` and ``char const* log_get_identifier()``.
 - ``FILE* LOG_FILE``: log messages are emitted to this stream.

As C and C++ code is increasingly written as "header-only" code, this logging tool is intended to support re-inclusion from header files. In order support this, the file must be included at the "top" of the header file, then it may be "unincluded" at the bottom, which will cause all conflicting macros to undefined.

```
#include <logging.h>
...
#define LOG_UNINCLUDE
#include <logging.h>
```

## Timer
There is also a `stealth_timing.h` header.
It adds a `class Timer` which records intervals of time between `start()` and `stop()`, as well as `count()` for the number of intervals it recorded.
 - `void start()` records the start of an interval.
 - `void stop()` records the end of an interval.

It also adds some getters for the results.
 - `unsigned long seconds() const` returns the total duration in seconds.
 - `unsigned long milliseconds() const` returns the total duration in milliseconds.
 - `unsigned long microseconds() const` returns the total duration in microseconds.
 - `unsigned long nanoseconds() const` returns the total duration in nanoseconds.
 - `unsigned long count() const` returns the total number of intervals.
