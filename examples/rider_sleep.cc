#include <unistd.h>
#include <benchmark/benchmark.h>

inline void  mssleep(int duration)
{
    usleep(duration*1000);
}

BENCHMARK(SomeSleep, Sleep1ms, 5, 10)
{
    mssleep(1);
}

BENCHMARK(SomeSleep, Sleep10ms, 5, 10)
{
    mssleep(10);
}

BENCHMARK(SomeSleep, Sleep20ms, 5, 10)
{
    mssleep(20);
}