#include <benchmark/benchmark.h>
#include <rider.h>

BENCHMARK_P(Rider, deliverPackage, 10, 100,
            (std::size_t speed, std::size_t distance))
{
    Rider(speed).deliverPackage(distance);
}

BENCHMARK_P_INSTANCE(Rider, deliverPackage, (1, 10));
BENCHMARK_P_INSTANCE(Rider, deliverPackage, (5, 10));
BENCHMARK_P_INSTANCE(Rider, deliverPackage, (10, 10));