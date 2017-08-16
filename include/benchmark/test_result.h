#ifndef BENCHMARK_TEST_RESULT_H_
#define BENCHMARK_TEST_RESULT_H_
#include <benchmark/clock.h>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <algorithm>

namespace benchmark {
class TestResult{
public: 
    TestResult(const std::vector<uint64_t>& run_times, 
                std::size_t iterations)
                :_runTimes(run_times),
                 _iterations(iterations),
                 _timeTotal(0),
                 _timeRunMin(std::numeric_limits<uint64_t>::max()),
                 _timeRunMax(std::numeric_limits<uint64_t>::min()),
                 _timeStdDev(0.0),
                 _timeMedian(0.0),
                 _timeQuartile1(0.0),
                 _timeQuartile3(0.0)
    {
        std::vector<uint64_t>::iterator runIt = _runTimes.begin();

        while (runIt != _runTimes.end()) {
            const uint64_t run = *runIt;
            _timeTotal += run;
            if ((runIt == _runTimes.begin()) || (run > _timeRunMax)) {
                _timeRunMax = run;
            }
            if ((runIt == _runTimes.begin()) || (run < _timeRunMin)) {
                _timeRunMin = run;
            }
            ++runIt;
        }
        // Calculate standard deviation.
        const double mean = runTimeAverage();
        double accu = 0.0;
        runIt = _runTimes.begin();
        while (runIt != _runTimes.end()) {
            const uint64_t run = *runIt;
            const double diff = double(run) - mean;
            accu += (diff * diff);
            ++runIt;
        }

        _timeStdDev = std::sqrt(accu / (_runTimes.size() - 1));

        // Calculate quartiles.
        std::vector<uint64_t> sortedRunTimes(_runTimes);
        std::sort(sortedRunTimes.begin(), sortedRunTimes.end());

        const std::size_t sortedSize = sortedRunTimes.size();
        const std::size_t sortedSizeHalf = sortedSize / 2;

        if (sortedSize >= 2) {
            const std::size_t quartile = sortedSizeHalf / 2;

            if ((sortedSize % 2) == 0) {
                _timeMedian =
                    (double(sortedRunTimes[sortedSizeHalf - 1]) +
                     double(sortedRunTimes[sortedSizeHalf])) / 2;

                _timeQuartile1 =
                    double(sortedRunTimes[quartile]);
                _timeQuartile3 =
                    double(sortedRunTimes[sortedSizeHalf + quartile]);
            } else {
                _timeMedian = double(sortedRunTimes[sortedSizeHalf]);
                _timeQuartile1 = (double(sortedRunTimes[quartile - 1]) +
                            double(sortedRunTimes[quartile])) / 2;
                _timeQuartile3 = (double(sortedRunTimes[sortedSizeHalf + (quartile - 1)]) +
                                 double(sortedRunTimes[sortedSizeHalf + quartile])) / 2;
            }
        } else if (sortedSize > 0) {
            _timeQuartile1 = double(sortedRunTimes[0]);
            _timeQuartile3 = _timeQuartile1;
        }
    }
    /// Total time.
    inline double timeTotal() const
    {
        return static_cast<double>(_timeTotal);
    }
    
    inline const std::vector<uint64_t>& runTimes() const
    {
        return _runTimes;
    }


    /// Average time per run.
    inline double runTimeAverage() const
    {
        return static_cast<double>(_timeTotal) / static_cast<double>(_runTimes.size());
    }

    /// Standard deviation time per run.
    inline double runTimeStdDev() const
    {
        return _timeStdDev;
    }

    /// Median (2nd Quartile) time per run.
    inline double runTimeMedian() const
    {
        return _timeMedian;
    }

    /// 1st Quartile time per run.
    inline double runTimeQuartile1() const
    {
        return _timeQuartile1;
    }

    /// 3rd Quartile time per run.
    inline double runTimeQuartile3() const
    {
        return _timeQuartile3;
    }

    /// Maximum time per run.
    inline double runTimeMaximum() const
    {
        return double(_timeRunMax);
    }


    /// Minimum time per run.
    inline double runTimeMinimum() const
    {
        return double(_timeRunMin);
    }


    /// Average runs per second.
    inline double runsPerSecondAverage() const
    {
        return 1000000000.0 / runTimeAverage();
    }

    /// Median (2nd Quartile) runs per second.
    inline double runsPerSecondMedian() const
    {
        return 1000000000.0 / runTimeMedian();
    }

    /// 1st Quartile runs per second.
    inline double runsPerSecondQuartile1() const
    {
        return 1000000000.0 / runTimeQuartile1();
    }

    /// 3rd Quartile runs per second.
    inline double runsPerSecondQuartile3() const
    {
        return 1000000000.0 /runTimeQuartile3();
    }

    /// Maximum runs per second.
    inline double runsPerSecondMaximum() const
    {
        return 1000000000.0 / _timeRunMin;
    }


    /// Minimum runs per second.
    inline double runsPerSecondMinimum() const
    {
        return 1000000000.0 / _timeRunMax;
    }


    /// Average time per iteration.
    inline double iterationTimeAverage() const
    {
        return runTimeAverage() / double(_iterations);
    }

    /// Standard deviation time per iteration.
    inline double iterationTimeStdDev() const
    {
        return runTimeStdDev() / static_cast<double>(_iterations);
    }

    /// Median (2nd Quartile) time per iteration.
    inline double iterationTimeMedian() const
    {
        return runTimeMedian() / static_cast<double>(_iterations);
    }

    /// 1st Quartile time per iteration.
    inline double iterationTimeQuartile1() const
    {
        return runTimeQuartile1() / static_cast<double>(_iterations);
    }

    /// 3rd Quartile time per iteration.
    inline double iterationTimeQuartile3() const
    {
        return runTimeQuartile3() / static_cast<double>(_iterations);
    }

    /// Minimum time per iteration.
    inline double iterationTimeMinimum() const
    {
        return _timeRunMin / static_cast<double>(_iterations);
    }


    /// Maximum time per iteration.
    inline double iterationTimeMaximum() const
    {
        return _timeRunMax / static_cast<double>(_iterations);
    }


    /// Average iterations per second.
    inline double iterationsPerSecondAverage() const
    {
        return 1000000000.0 / iterationTimeAverage();
    }

    /// Median (2nd Quartile) iterations per second.
    inline double iterationsPerSecondMedian() const
    {
        return 1000000000.0 / iterationTimeMedian();
    }

    /// 1st Quartile iterations per second.
    inline double iterationsPerSecondQuartile1() const
    {
        return 1000000000.0 / iterationTimeQuartile1();
    }

    /// 3rd Quartile iterations per second.
    inline double iterationsPerSecondQuartile3() const
    {
        return 1000000000.0 / iterationTimeQuartile3();
    }

    /// Minimum iterations per second.
    inline double iterationsPerSecondMinimum() const
    {
        return 1000000000.0 / iterationTimeMaximum();
    }


    /// Maximum iterations per second.
    inline double iterationsPerSecondMaximum() const
    {
        return 1000000000.0 / iterationTimeMinimum();
    }

private:
    std::vector<uint64_t>     _runTimes;
    std::size_t               _iterations;
    uint64_t                  _timeTotal;
    uint64_t                  _timeRunMin;
    uint64_t                  _timeRunMax;
    double                    _timeStdDev;
    double                    _timeMedian;
    double                    _timeQuartile1;
    double                    _timeQuartile3;
};
}

#endif
