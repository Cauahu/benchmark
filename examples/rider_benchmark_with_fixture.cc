#include <benchmark/benchmark.h>
#include <rider.h>

class SlowRiderFixture : public ::benchmark::Fixture {
public:
    virtual void setUp()
    {
        this->SlowRider = new Rider(1);
    }

    virtual void tearDown()
    {
        delete this->SlowRider;
    }

    Rider* SlowRider;
};

BENCHMARK_F(SlowRiderFixture, deliverPackage, 10, 100)
{
    SlowRider->deliverPackage(10);
}
