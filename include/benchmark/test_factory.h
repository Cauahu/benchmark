#ifndef BENCHMARK_TEST_FACTORY_H_
#define BENCHMARK_TEST_FACTORY_H_
#include <benchmark/test.h>

namespace benchmark {

class TestFactory {
public:
    virtual ~TestFactory()
    {

    }
    virtual Test* createTest() = 0;
};
}
#endif