#include <benchmark/benchmark_main.h>

int main(int argc, char** argv)
{
    ::benchmark::MainRunner runner;
    int result = runner.ParseArgs(argc, argv);
    if(result) {
        return result;
    }
    return runner.run();
}