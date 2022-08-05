#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define LOG_ENABLE_TRACE
#define LOG_ENABLE_DEBUG
#include "stealth_logging.h"

int main()
{
  log_set_identifier("test main");

  log_trace("trace message %s", "trace");
  log_debug("debug message %s", "debug");
  log_info("info message %s", "info");
  log_warn("warn message %s", "warn");
  log_error("error message %s", "error");

  FILE* f = fopen("not_a_file", "rw");
  if(f == NULL)
  {
    log_perror();
  }

#ifdef _WIN32
  HANDLE h = CreateFileA("not_a_file", GENERIC_READ, 0, NULL, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL, NULL);
  if(h == INVALID_HANDLE_VALUE)
  {
    log_wperror();
  }
#endif

  log_assert(false, "assert message %s", "assert");

  return 0;
}
