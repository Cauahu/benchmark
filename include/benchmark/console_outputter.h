#ifndef BENCHMARK_CONSOLE_OUTPUTTER_H_
#define BENCHMARK_CONSOLE_OUTPUTTER_H_
#include <benchmark/console.h>
#include <benchmark/outputter.h>

namespace benchmark {
class ConsoleOutputter : public Outputter {
public:

    ConsoleOutputter(std::ostream& stream = std::cout)
        :   _stream(stream)
    {

    }


    virtual void begin(const std::size_t& enabledCount,
                        const std::size_t& disabledCount)
    {
        _stream << std::fixed;
        _stream << Console::TextGreen << "[==========]"
                << Console::TextDefault << " Running "
                << enabledCount
                << (enabledCount == 1 ? " benchmark." : " benchmarks");

        if (disabledCount) {
            _stream << ", skipping "
                    << disabledCount
                    << (disabledCount == 1 ?
                        " benchmark." :
                        " benchmarks");
        } else {
            _stream << ".";
        }
        _stream << std::endl;
    }


    virtual void end(const std::size_t& executedCount,
                         const std::size_t& disabledCount)
    {
        _stream << Console::TextGreen << "[==========]"
                << Console::TextDefault << " Ran " << executedCount
                << (executedCount == 1 ?
                    " benchmark." :
                    " benchmarks");

            if (disabledCount) {
                _stream << ", skipped "
                        << disabledCount
                        << (disabledCount == 1 ?
                            " benchmark." :
                            " benchmarks");
            } else {
                _stream << ".";
            }
            _stream << std::endl;
        }


        inline void beginOrSkipTest(const std::string& fixtureName,
                                    const std::string& testName,
                                    const TestParametersDescriptor& parameters,
                                    const std::size_t& runsCount,
                                    const std::size_t& iterationsCount,
                                    const bool skip)
        {
            if (skip) {
                _stream << Console::TextCyan << "[ DISABLED ]";
            } else {
                _stream << Console::TextGreen << "[ RUN      ]";
            }
            _stream << Console::TextYellow << " ";
            writeTestNameToStream(_stream, fixtureName, testName, parameters);
            _stream << Console::TextDefault
                    << " (" << runsCount
                    << (runsCount == 1 ? " run, " : " runs, ")
                    << iterationsCount
                    << (iterationsCount == 1 ?
                        " iteration per run)" :
                        " iterations per run)")
                    << std::endl;
        }


        virtual void beginTest(const std::string& fixtureName,
                               const std::string& testName,
                               const TestParametersDescriptor& parameters,
                               const std::size_t& runsCount,
                               const std::size_t& iterationsCount)
        {
            beginOrSkipTest(fixtureName,
                            testName,
                            parameters,
                            runsCount,
                            iterationsCount,
                            false);
        }


        virtual void skipDisabledTest(
            const std::string& fixtureName,
            const std::string& testName,
            const TestParametersDescriptor& parameters,
            const std::size_t& runsCount,
            const std::size_t& iterationsCount
        )
        {
            beginOrSkipTest(fixtureName,
                            testName,
                            parameters,
                            runsCount,
                            iterationsCount,
                            true);
        }


        virtual void endTest(const std::string& fixtureName,
                             const std::string& testName,
                             const TestParametersDescriptor& parameters,
                             const TestResult& result)
        {
#define PAD(x) _stream << std::setw(34) << x << std::endl;
#define PAD_DEVIATION(description,                                      \
                      deviated,                                         \
                      average,                                          \
                      unit)                                             \
            {                                                           \
                double _d_ =                                            \
                    double(deviated) - double(average);                 \
                                                                        \
                PAD(description <<                                      \
                    deviated << " " << unit << " (" <<                  \
                    (deviated < average ?                               \
                     Console::TextRed :                                 \
                     Console::TextGreen) <<                             \
                    (deviated > average ? "+" : "") <<                  \
                    _d_ << " " << unit << " / " <<                      \
                    (deviated > average ? "+" : "") <<                  \
                    (_d_ * 100.0 / average) << " %" <<                  \
                    Console::TextDefault << ")");                       \
            }
#define PAD_DEVIATION_INVERSE(description,                              \
                              deviated,                                 \
                              average,                                  \
                              unit)                                     \
            {                                                           \
                double _d_ =                                            \
                    double(deviated) - double(average);                 \
                                                                        \
                PAD(description <<                                      \
                    deviated << " " << unit << " (" <<                  \
                    (deviated > average ?                               \
                     Console::TextRed :                                 \
                     Console::TextGreen) <<                             \
                    (deviated > average ? "+" : "") <<                  \
                    _d_ << " " << unit << " / " <<                      \
                    (deviated > average ? "+" : "") <<                  \
                    (_d_ * 100.0 / average) << " %" <<                  \
                    Console::TextDefault << ")");                       \
            }

            _stream << Console::TextGreen << "[     DONE ]"
                    << Console::TextYellow << " ";
            writeTestNameToStream(_stream, fixtureName, testName, parameters);
            _stream << Console::TextDefault << " ("
                    << std::setprecision(6)
                    << (result.timeTotal() / 1000000.0) << " ms)"
                    << std::endl;

            _stream << Console::TextBlue << "[   RUNS   ] "
                    << Console::TextDefault
                    << "       Average time: "
                    << std::setprecision(3)
                    << result.runTimeAverage() / 1000.0 << " us "
                    << "(" << Console::TextBlue << "~"
                    << result.runTimeStdDev() / 1000.0 << " us"
                    << Console::TextDefault << ")"
                    << std::endl;

            PAD_DEVIATION_INVERSE("Fastest time: ",
                                  (result.runTimeMinimum() / 1000.0),
                                  (result.runTimeAverage() / 1000.0),
                                  "us");
            PAD_DEVIATION_INVERSE("Slowest time: ",
                                  (result.runTimeMaximum() / 1000.0),
                                  (result.runTimeAverage() / 1000.0),
                                  "us");
            PAD("Median time: " <<
                result.runTimeMedian() / 1000.0 << " us (" <<
                Console::TextCyan << "1st quartile: " <<
                result.runTimeQuartile1() / 1000.0 << " us | 3rd quartile: " <<
                result.runTimeQuartile3() / 1000.0 << " us" <<
                Console::TextDefault << ")");

            _stream << std::setprecision(5);

            PAD("");
            PAD("Average performance: " <<
                result.runsPerSecondAverage() << " runs/s");
            PAD_DEVIATION("Best performance: ",
                          result.runsPerSecondMaximum(),
                          result.runsPerSecondAverage(),
                          "runs/s");
            PAD_DEVIATION("Worst performance: ",
                          result.runsPerSecondMinimum(),
                          result.runsPerSecondAverage(),
                          "runs/s");
            PAD("Median performance: " <<
                result.runsPerSecondMedian() << " runs/s (" <<
                Console::TextCyan << "1st quartile: " <<
                result.runsPerSecondQuartile1() << " | 3rd quartile: " <<
                result.runsPerSecondQuartile3() <<
                Console::TextDefault << ")");

            PAD("");
            _stream << Console::TextBlue << "[ITERATIONS] "
                    << Console::TextDefault
                    << std::setprecision(3)
                    << "       Average time: "
                    << result.iterationTimeAverage() / 1000.0 << " us "
                    << "(" << Console::TextBlue << "~"
                    << result.iterationTimeStdDev() / 1000.0 << " us"
                    << Console::TextDefault << ")"
                    << std::endl;

            PAD_DEVIATION_INVERSE("Fastest time: ",
                                  (result.iterationTimeMinimum() / 1000.0),
                                  (result.iterationTimeAverage() / 1000.0),
                                  "us");
            PAD_DEVIATION_INVERSE("Slowest time: ",
                                  (result.iterationTimeMaximum() / 1000.0),
                                  (result.iterationTimeAverage() / 1000.0),
                                  "us");
            PAD("Median time: " <<
                result.iterationTimeMedian() / 1000.0 << " us (" <<
                Console::TextCyan << "1st quartile: " <<
                result.iterationTimeQuartile1() / 1000.0 <<
                " us | 3rd quartile: " <<
                result.iterationTimeQuartile3() / 1000.0 << " us" <<
                Console::TextDefault << ")");

            _stream << std::setprecision(5);

            PAD("");
            PAD("Average performance: " <<
                result.iterationsPerSecondAverage() <<
                " iterations/s");
            PAD_DEVIATION("Best performance: ",
                          (result.iterationsPerSecondMaximum()),
                          (result.iterationsPerSecondAverage()),
                          "iterations/s");
            PAD_DEVIATION("Worst performance: ",
                          (result.iterationsPerSecondMinimum()),
                          (result.iterationsPerSecondAverage()),
                          "iterations/s");
            PAD("Median performance: " <<
                result.iterationsPerSecondMedian() << " iterations/s (" <<
                Console::TextCyan << "1st quartile: " <<
                result.iterationsPerSecondQuartile1() <<
                " | 3rd quartile: " <<
                result.iterationsPerSecondQuartile3() <<
                Console::TextDefault << ")");

#undef PAD_DEVIATION_INVERSE
#undef PAD_DEVIATION
#undef PAD
        }


        std::ostream& _stream;
    };
}
#endif
