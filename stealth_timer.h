/**
 * Copyright Stealth Software Technologies, Inc.
 */

#ifndef STEALTH_LOGGING_TIMER_H_
#define STEALTH_LOGGING_TIMER_H_

// #include <chrono>
#include <cstdint>
#include <ctime>

class Timer
{
#ifdef LOG_OPT_ENABLE_TIMERS

  // total duration
  long unsigned duration = 0;
  // time of most recent start()
  struct timespec startTime;
  // total count of stop()
  long unsigned counter = 0;

#endif

  static constexpr long unsigned SEC_TO_NSEC = 1000000000;
  static constexpr long unsigned NSEC_TO_SEC = 1000000000;
  static constexpr long unsigned NSEC_TO_MSEC = 1000000;
  static constexpr long unsigned NSEC_TO_USEC = 1000;

public:
  inline void start()
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    clock_gettime(CLOCK_MONOTONIC, &this->startTime);
#endif
  };

  inline void stop()
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    this->duration +=
      (long unsigned) (end.tv_sec - this->startTime.tv_sec) * SEC_TO_NSEC
      + (long unsigned) end.tv_nsec - (long unsigned) this->startTime.tv_nsec;
    counter++;
#endif
  }

  long unsigned seconds() const
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    return this->duration / NSEC_TO_SEC;
#else
    return 0;
#endif
  }

  long unsigned milliseconds() const
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    return this->duration / NSEC_TO_MSEC;
#else
    return 0;
#endif
  }

  long unsigned microseconds() const
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    return this->duration / NSEC_TO_USEC;
#else
    return 0;
#endif
  }

  long unsigned nanoseconds() const
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    return this->duration;
#else
    return 0;
#endif
  }

  long unsigned count() const
  {
#ifdef LOG_OPT_ENABLE_TIMERS
    return this->counter;
#else
    return 0;
#endif
  }
};

#endif//STEALTH_LOGGING_TIMER_H_
