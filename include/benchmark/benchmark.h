#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_
#include <benchmark/test.h>
#include <benchmark/benchmarker.h>
#include <benchmark/default_test_factory.h>
#include <benchmark/fixture.h>
#include <benchmark/console_outputter.h>
#include <benchmark/clock.h>

#define BENCHMARK_VERSION "1.0.0"

#define BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) \
    fixture_name ## _ ## benchmark_name ## _Benchmark

#define BENCHMARK_(fixture_name,                                        \
                   benchmark_name,                                      \
                   fixture_class_name,                                  \
                   runs,                                                \
                   iterations)                                          \
    class BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)           \
        :   public fixture_class_name                                   \
    {                                                                   \
    public:                                                             \
        BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)()           \
        {                                                               \
                                                                        \
        }                                                               \
    protected:                                                          \
        virtual void testBody();                                        \
    private:                                                            \
        static const ::benchmark::TestDescriptor* _descriptor;              \
    };                                                                  \
                                                                        \
    const ::benchmark::TestDescriptor*                                      \
    BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_descriptor =  \
        ::benchmark::BenchMarker::instance().registerTest(                  \
            #fixture_name,                                              \
            #benchmark_name,                                            \
            runs,                                                       \
            iterations,                                                 \
            new ::benchmark::TestFactoryDefault<                            \
                BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)     \
            >(),                                                        \
            ::benchmark::TestParametersDescriptor());                       \
                                                                        \
    void BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::testBody()

#define BENCHMARK_F(fixture_name,                        \
                    benchmark_name,                      \
                    runs,                                \
                    iterations)                          \
    BENCHMARK_(fixture_name,                             \
               benchmark_name,                           \
               fixture_name,                             \
               runs,                                     \
               iterations)

#define BENCHMARK(fixture_name,                          \
                  benchmark_name,                        \
                  runs,                                  \
                  iterations)                            \
    BENCHMARK_(fixture_name,                             \
               benchmark_name,                           \
               ::benchmark::Test,                            \
               runs,                                     \
               iterations)

#define BENCHMARK_P_(fixture_name,                                      \
                     benchmark_name,                                    \
                     fixture_class_name,                                \
                     runs,                                              \
                     iterations,                                        \
                     arguments)                                         \
    class BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)           \
        :   public fixture_class_name {                                 \
    public:                                                             \
        BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) () {}       \
        virtual ~ BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) () {} \
        static const std::size_t _runs = runs;                          \
        static const std::size_t _iterations = iterations;              \
        static const char* _argumentsDeclaration() { return #arguments;  } \
    protected:                                                          \
        inline void TestPayload arguments;                              \
    };                                                                  \
    void BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::TestPayload arguments

#define BENCHMARK_P(fixture_name,               \
                    benchmark_name,             \
                    runs,                       \
                    iterations,                 \
                    arguments)                  \
    BENCHMARK_P_(fixture_name,                  \
                 benchmark_name,                \
                 ::benchmark::Fixture,                \
                 runs,                          \
                 iterations,                    \
                 arguments)

#define BENCHMARK_P_F(fixture_name, benchmark_name, runs, iterations, arguments) \
        BENCHMARK_P_(fixture_name, benchmark_name, fixture_name, runs, iterations, arguments)

#define BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id)   \
        fixture_name ## _ ## benchmark_name ## _Benchmark_ ## id

#define BENCHMARK_P_INSTANCE1(fixture_name, benchmark_name, arguments, id) \
    class BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id):    \
        public BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name) {    \
    protected:                                                          \
        virtual void testBody() { this->TestPayload arguments; }        \
    private:                                                            \
        static const ::benchmark::TestDescriptor* _descriptor;              \
    };                                                                  \
    const ::benchmark::TestDescriptor* BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id)::_descriptor = \
        ::benchmark::BenchMarker::instance().registerTest(                  \
            #fixture_name, #benchmark_name,                             \
            BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_runs, \
            BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_iterations, \
            new ::benchmark::TestFactoryDefault< BENCHMARK_P_CLASS_NAME_(fixture_name, benchmark_name, id) >(), \
            ::benchmark::TestParametersDescriptor(BENCHMARK_CLASS_NAME_(fixture_name, benchmark_name)::_argumentsDeclaration(), #arguments))

#if defined(__COUNTER__)
#   define BENCHMARK_P_ID_ __COUNTER__
#else
#   define BENCHMARK_P_ID_ __LINE__
#endif

#define BENCHMARK_P_INSTANCE(fixture_name, benchmark_name, arguments)   \
    BENCHMARK_P_INSTANCE1(fixture_name, benchmark_name, arguments, BENCHMARK_P_ID_)




#endif
