#ifndef BENCHMARK_TEST_H_
#define BENCHMARK_TEST_H_
#include <cstddef>
#include <benchmark/clock.h>
#include <benchmark/test_result.h>
#include <benchmark/clock.h>
namespace benchmark{

class Test {
public:
    virtual void setUp()
    {

    }

    virtual void tearDown()
    {

    }

    uint64_t run(std::size_t iterations)
    {
        std::size_t iteration = iterations;
            
        // Set up the testing fixture.
        setUp();

        // Get the starting time.
        Clock::TimePoint startTime;
        Clock::TimePoint endTime;

        startTime = Clock::now();

        // Run the test body for each iteration.
        while (iteration--)
                testBody();

        // Get the ending time.
        endTime = Clock::now();

        // Tear down the testing fixture.
        tearDown();

        // Return the duration in nanoseconds.
        return Clock::duration(startTime, endTime);
    }
    virtual ~Test()
    {
        
    }
protected: 
    virtual void testBody()
    {

    }
};

}
#endif