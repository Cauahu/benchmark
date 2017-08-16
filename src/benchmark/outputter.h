#ifndef BENCHMARK_OUTPUTTER_H_
#define BENCHMARK_OUTPUTTER_H_
#include <iostream>
#include <cstddef>
#include <benchmark/test_result.h>

namespace benchmark {

class Outputter {
public:
    virtual void begin(const std::size_t& enabledCount,
                           const std::size_t& disabledCount) = 0;

    virtual void end(const std::size_t& executedCount,
                         const std::size_t& disabledCount) = 0;  

    virtual void beginTest(const std::string& fixtureName,
                               const std::string& testName,
                               const TestParametersDescriptor& parameters,
                               const std::size_t& runsCount,
                               const std::size_t& iterationsCount) = 0;

    virtual void endTest(const std::string& fixtureName,
                             const std::string& testName,
                             const TestParametersDescriptor& parameters,
                             const TestResult& result) = 0;

    virtual void skipDisabledTest(const std::string& fixtureName,
                                      const std::string& testName,
                                      const TestParametersDescriptor &parameters,
                                      const std::size_t& runsCount,
                                      const std::size_t& iterationsCount) = 0;
    
    virtual ~Outputter()
    {

    }
protected:
     static void writeTestNameToStream(std::ostream& stream,
                                          const std::string& fixtureName,
                                          const std::string& testName,
                                          const TestParametersDescriptor& parameters)
        {
            stream << fixtureName << "." << testName;

            const std::vector<TestParameterDescriptor>& descs =
                parameters.Parameters();

            if (descs.empty()) {
                return;
            }
            stream << "(";

            for (std::size_t i = 0; i < descs.size(); ++i) {
                if (i) {
                    stream << ", ";
                }

                const TestParameterDescriptor& desc = descs[i];
                stream << desc.Declaration << " = " << desc.Value;
            }

            stream << ")";
        }

                 
};

}
#endif