#ifndef BENCHMARK_BENCHMARKER_H_
#define BENCHMARK_BENCHMARKER_H_
#include <algorithm>
#include <vector>
#include <limits>
#include <iomanip>
#include <string>
#include <cstring>
#include <assert.h>
#include <benchmark/test_factory.h>
#include <benchmark/test_descriptor.h>
#include <benchmark/test_result.h>
#include <benchmark/console_outputter.h>

namespace benchmark {

class BenchMarker {
public:
    static BenchMarker& instance()
    {
        static BenchMarker singleton;
        return singleton;
    }

    static TestDescriptor* registerTest(const char* fixtureName,
            const char* testName,
            std::size_t runs,
            std::size_t iterations,
            TestFactory* testFactory,
            TestParametersDescriptor parameters)
    {
        static const char* disabledPrefix = "DISABLED_";
        bool isDisabled = ((::strlen(testName) >= 9) &&
                               (!::memcmp(testName, disabledPrefix, 9)));

        if (isDisabled) {
            testName += 9;
        }

        // Add the descriptor.
        TestDescriptor* descriptor = new TestDescriptor(fixtureName,
                                                        testName,
                                                        runs,
                                                        iterations,
                                                        testFactory,
                                                        parameters,
                                                        isDisabled);

        instance()._tests.push_back(descriptor);
  
        return descriptor;
    }

    static void addOutputter(Outputter & out)
    {
        instance()._outputters.push_back(&out);  
    }

    static void applyPatternFilter(const char* pattern)
    {
        BenchMarker& ins = instance();

        // Split the filter at '-' if it exists.
        const char* const dash = strchr(pattern, '-');

        std::string positive;
        std::string negative;

        if (dash == NULL) {
            positive = pattern;
        } else {
            positive = std::string(pattern, dash);
            negative = std::string(dash + 1);
            if (positive.empty())
                positive = "*";
        }

        // Iterate across all tests and test them against the patterns.
        std::size_t index = 0;
        while (index < ins._tests.size()) {
            TestDescriptor* desc = ins._tests[index];

            if ((!filterMatchesString(positive.c_str(),
                                        desc->CanonicalName)) ||
                (filterMatchesString(negative.c_str(),
                                        desc->CanonicalName))) {
                ins._tests.erase(
                    ins._tests.begin() +
                    std::vector<TestDescriptor*>::difference_type(index)
                );
                delete desc;
            } else {
                ++index;
            }
        }
    }

    static void runAllTests()
    {
        ConsoleOutputter          defaultOutputter;
        std::vector<Outputter*>   defaultOutputters;
        BenchMarker              &ins = instance();
        defaultOutputters.push_back(&defaultOutputter);
        std::vector<Outputter*>& outputters =
                (ins._outputters.empty() ?
                 defaultOutputters :
                 ins._outputters);

        // Get the tests for execution.
        std::vector<TestDescriptor*> tests = ins.getTests();

        const std::size_t totalCount = tests.size();
        std::size_t disabledCount = 0;

        std::vector<TestDescriptor*>::const_iterator testsIt = 
                    tests.begin();

        while (testsIt != tests.end()) {
            if ((*testsIt)->IsDisabled) {
                    ++disabledCount;
            }
            ++testsIt;
        }

        const std::size_t enabledCount = totalCount - disabledCount;

        // Calibrate the tests.
        const CalibrationModel calibrationModel = getCalibrationModel();

        // Begin output.
        for (std::size_t outputterIndex = 0;
                outputterIndex < outputters.size();
                outputterIndex++)
                outputters[outputterIndex]->begin(enabledCount, disabledCount);

        // Run through all the tests in ascending order.
        std::size_t index = 0;

        while (index < tests.size()) {
            // Get the test descriptor.
            TestDescriptor* descriptor = tests[index++];

            // Check if test matches include filters
            if (ins._include.size() > 0) {
                bool included = false;
                std::string name =
                descriptor->FixtureName + "." +
                descriptor->TestName;
                for (std::size_t i = 0; i < ins._include.size(); i++) {
                    if (name.find(ins._include[i]) !=
                            std::string::npos) {
                        included = true;
                        break;
                    }
                }

                if (!included) {
                        continue;
                }
            }

            // Check if test is not disabled.
            if (descriptor->IsDisabled) {
                for (std::size_t outputterIndex = 0;
                        outputterIndex < outputters.size();
                        outputterIndex++) {
                    outputters[outputterIndex]->skipDisabledTest(
                        descriptor->FixtureName,
                        descriptor->TestName,
                        descriptor->Parameters,
                        descriptor->Runs,
                        descriptor->Iterations
                    );
                }

                continue;
            }

            // Describe the beginning of the run.
            for (std::size_t outputterIndex = 0;
                     outputterIndex < outputters.size();
                     outputterIndex++) {
                outputters[outputterIndex]->beginTest(
                    descriptor->FixtureName,
                    descriptor->TestName,
                    descriptor->Parameters,
                    descriptor->Runs,
                    descriptor->Iterations
                );
            }

            // Execute each individual run.
            std::vector<uint64_t> runTimes(descriptor->Runs);
            uint64_t overheadCalibration =
                    calibrationModel.getCalibration(descriptor->Iterations);

            std::size_t run = 0;
            while (run < descriptor->Runs) {
                // Construct a test instance.
                Test* test = descriptor->Factory->createTest();

                // Run the test.
                uint64_t time = test->run(descriptor->Iterations);

                // Store the test time.
                runTimes[run] = (time > overheadCalibration ?
                                    time - overheadCalibration :
                                    0);

                // Dispose of the test instance.
                delete test;

                ++run;
            }

            // Calculate the test result.
            TestResult testResult(runTimes, descriptor->Iterations);

            // Describe the end of the run.
            for (std::size_t outputterIndex = 0;
                     outputterIndex < outputters.size();
                     outputterIndex++)
                    outputters[outputterIndex]->endTest(
                        descriptor->FixtureName,
                        descriptor->TestName,
                        descriptor->Parameters,
                        testResult
                    );

            }

            // End output.
            for (std::size_t outputterIndex = 0;
                 outputterIndex < outputters.size();
                 outputterIndex++) {
                outputters[outputterIndex]->end(enabledCount,
                                                disabledCount);
            }
        }

        static std::vector<const TestDescriptor*> listTests()
        {
            std::vector<const TestDescriptor*> tests;
            BenchMarker& ins = instance();

            std::size_t index = 0;
            while (index < ins._tests.size()) {
                tests.push_back(ins._tests[index++]);
            }

            return tests;
        }

         static void shuffleTests()
        {
            BenchMarker& ins = instance();
            std::random_shuffle(ins._tests.begin(),
                                ins._tests.end());
        }

private:
    struct CalibrationModel {
    public:
        CalibrationModel(std::size_t scale,
                            uint64_t slope,
                            uint64_t yIntercept)
            : Scale(scale),
              Slope(slope),
              YIntercept(yIntercept)
        {

        }

        const std::size_t Scale;

        const uint64_t Slope;

        const uint64_t YIntercept;

        int64_t getCalibration(std::size_t iterations) const
        {
            return YIntercept + (iterations * Slope) / Scale;
        }
    };
private:  
    BenchMarker()
    {

    }

    ~BenchMarker()
    {
        // Release all test descriptors.
        std::size_t index = _tests.size();
        while (index--) {
            delete _tests[index];
        }
    }

    std::vector<TestDescriptor*> getTests() const
    {
        std::vector<TestDescriptor*> tests;

        std::size_t index = 0;
        while (index < _tests.size()){
            tests.push_back(_tests[index++]);
        }
        return tests;
    }


        /// Test if a filter matches a string.

        /// Adapted from gtest. All rights reserved by original authors.
    static bool filterMatchesString(const char* filter,
                                    const std::string& str)
    {
        const char *patternStart = filter;

        while (true) {
            if (patternMatchesString(patternStart, str.c_str())){
                return true;
            }
            // Finds the next pattern in the filter.
            patternStart = strchr(patternStart, ':');

            // Returns if no more pattern can be found.
            if (!patternStart) {
                return false;
            }
            // Skips the pattern separater (the ':' character).
            patternStart++;
        }
    }


    /// Test if pattern matches a string.

    /// Adapted from gtest. All rights reserved by original authors.
    static bool patternMatchesString(const char* pattern, const char *str)
    {
        switch (*pattern) {
        case '\0':
        case ':':
            return (*str == '\0');
        case '?':  // Matches any single character.
            return ((*str != '\0') &&
                    (patternMatchesString(pattern + 1, str + 1)));
        case '*':  // Matches any string (possibly empty) of characters.
            return (((*str != '\0') &&
                        (patternMatchesString(pattern, str + 1))) ||
                    (patternMatchesString(pattern + 1, str)));
        default:
            return ((*pattern == *str) &&
                    (patternMatchesString(pattern + 1, str + 1)));
        }
    }

 /// Get calibration model.

        /// Returns an average linear calibration model.
    static CalibrationModel getCalibrationModel()
    {
        // We perform a number of runs of varying iterations with an empty
        // test body. The assumption here is, that the time taken for the
        // test run is linear with regards to the number of iterations, ie.
        // some constant overhead with a per-iteration overhead. This
        // hypothesis has been manually validated by linear regression over
        // sample data.
        //
        // In order to avoid losing too much precision, we are going to
        // calibrate in terms of the overhead of some x n iterations,
        // where n must be a sufficiently large number to produce some
        // significant runtime. On a high-end 2012 Retina MacBook Pro with
        // -O3 on clang-602.0.53 (LLVM 6.1.0) n = 1,000,000 produces
        // run times of ~1.9 ms, which should be sufficiently precise.
        //
        // However, as the constant overhead is mostly related to
        // retrieving the system clock, which under the same conditions
        // clocks in at around 17 ms, we run the risk of winding up with
        // a negative y-intercept if we do not fix the y-intercept. This
        // intercept is therefore fixed by a large number of runs of 0
        // iterations.
        ::benchmark::Test* test = new Test();

#define CALIBRATION_INTERESECT_RUNS 10000

#define CALIBRATION_RUNS 10
#define CALIBRATION_SCALE 1000000
#define CALIBRATION_PPR 6

            // Determine the intercept.
        uint64_t
            interceptSum = 0,
            interceptMin = std::numeric_limits<uint64_t>::min(),
            interceptMax = 0;

        for (std::size_t run = 0;
                run < CALIBRATION_INTERESECT_RUNS;
                ++run) {
            uint64_t intercept = test->run(0);
            interceptSum += intercept;
            if (intercept < interceptMin) {
                interceptMin = intercept;
            }
            if (intercept > interceptMax) {
                interceptMax = intercept;
            }
        }

        uint64_t interceptAvg =
            interceptSum / CALIBRATION_INTERESECT_RUNS;

        // Produce a series of sample points.
        std::vector<uint64_t> x(CALIBRATION_RUNS *
                                CALIBRATION_PPR);
        std::vector<uint64_t> t(CALIBRATION_RUNS *
                                CALIBRATION_PPR);

        std::size_t point = 0;

        for (std::size_t run = 0; run < CALIBRATION_RUNS; ++run) {
#define CALIBRATION_POINT(_x)                                     \
            x[point] = _x;                                          \
            t[point++] =                                            \
                test->run(_x * std::size_t(CALIBRATION_SCALE))

            CALIBRATION_POINT(1);
            CALIBRATION_POINT(2);
            CALIBRATION_POINT(5);
            CALIBRATION_POINT(10);
            CALIBRATION_POINT(15);
            CALIBRATION_POINT(20);

#undef CALIBRATION_POINT
        }

        // As we have a fixed y-intercept, b, the optimal slope for a line
        // fitting the sample points will be
        // $\frac {\sum_{i=1}^{n} x_n \cdot (y_n - b)}
        //  {\sum_{i=1}^{n} {x_n}^2}$.
        uint64_t sumProducts = 0;
        uint64_t sumXSquared = 0;

        std::size_t p = x.size();
        while (p--) {
                sumXSquared += x[p] * x[p];
                sumProducts += x[p] * (t[p] - interceptAvg);
        }

        uint64_t slope = sumProducts / sumXSquared;

        delete test;

        return CalibrationModel(CALIBRATION_SCALE,
                                    slope,
                                    interceptAvg);

#undef CALIBRATION_INTERESECT_RUNS

#undef CALIBRATION_RUNS
#undef CALIBRATION_SCALE
#undef CALIBRATION_PPR
    }
private:
    std::vector<Outputter*>       _outputters; ///< Registered outputters.
    std::vector<TestDescriptor*>  _tests; ///< Registered tests.
    std::vector<std::string>      _include; ///< Test filters.


};

}
#endif
