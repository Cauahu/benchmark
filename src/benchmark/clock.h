#ifndef BENCHMARK_CLOCK_H_
#define BENCHMARK_CLOCK_H_

#include <benchmark/captibility.h>
#if defined(__APPLE__) || defined(__MACH__)
    #include <mach/mach_time.h>
#elif defined(__unix__) || defined(__unix) || defined(unix)
    #include <sys/time.h>
#endif

#include <stdexcept>
#include <stdint.h>

namespace benchmark {
#if defined(__APPLE__) && defined(__MACH__)
class Clock {
public:
    typedef uint64_t TimePoint;
    typedef uint64_t TimeDiff;

    static TimePoint now() bm_noexcept
    {
        return mach_absolute_time();
    }

    static TimeDiff duration(const TimePoint &start_time,
                             const TimePoint &end_time) bm_noexcept
    {
        mach_timebase_info_data_t time_info;
        mach_timebase_info(&time_info);

        return (end_time - start_time) * time_info.numer / time_info.denom;
    }

    static const char* description()
    {
        return "mach_abosulte_time";
    }
};
#else
class Clock {
public:
    typedef uint64_t TimePoint;
    typedef uint64_t TimeDiff;
    static const uint64_t kNanoPerSec = 1000000000;
    static const uint64_t kNanoPerUsec = 1000;
    static TimePoint now() bm_noexcept
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * kNanoPerSec + tv.tv_usec * kNanoPerUsec;
    }

    static TimeDiff duration(const TimePoint &start_time,
                             const TimePoint &end_time) bm_noexcept
    {
         return (end_time - start_time);   
    }

    static const char* description()
    {
        return "gettimeofday";
    }
};
#endif

}
#endif