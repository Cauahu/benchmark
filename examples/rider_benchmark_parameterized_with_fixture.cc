#include <benchmark/benchmark.h>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <rider.h>

class FastRiderFixture
    :   public ::benchmark::Fixture
{
public:
    virtual void setUp()
    {
        this->FastRider = new Rider(10);
    }

    virtual void tearDown()
    {
        delete this->FastRider;
    }

    Rider* FastRider;
};

/*
 * Note _F suffix in macro name.
 */
BENCHMARK_P_F(FastRiderFixture, deliverPackage, 10, 100,
              (std::size_t distance))
{
    FastRider->deliverPackage(distance);
}

BENCHMARK_P_INSTANCE(FastRiderFixture, deliverPackage, (1));
BENCHMARK_P_INSTANCE(FastRiderFixture, deliverPackage, (10));
BENCHMARK_P_INSTANCE(FastRiderFixture, deliverPackage, (100));