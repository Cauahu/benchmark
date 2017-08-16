#ifndef BENCHMARK_DEFAULT_TEST_FACTORY_H_
#define BENCHMARK_DEFAULT_TEST_FACTORY_H_
#include <benchmark/test_factory.h>

namespace benchmark {
 template<class T>
    class TestFactoryDefault : public TestFactory {
    public:
    
        virtual Test* createTest()
        {
            return new T();
        }
    };
}
#endif
