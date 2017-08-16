#include <benchmark/benchmark.h>
#include <rider.h>

BENCHMARK(Rider, deliverPackage, 10, 100)
{
    Rider(1).deliverPackage(100);
}

BENCHMARK(Rider, DISABLED_deliverPackage, 10, 10000)
{
    Rider(1).deliverPackage(10000);
}
