file(GLOB BENCHMARK_HEADERS
  benchmark/benchmark.h
  benchmark/benchmarker.h
  benchmark/clock.h
  benchmark/compatibility.h
  benchmark/console.h
  benchmark/console_outputter.h
  benchmark/default_test_factory.h
  benchmark/fixture.h
  benchmark/outputter.h
  benchmark/test.h
  benchmark/test_descriptor.h
  benchmark/test_factory.h
  benchmark/test_result.h
  benchmark/benchmark_main.h
)

add_library(benchmark_main benchmark/benchmark_main.cc)

#set_target_properties(main PROPERTIES
# PUBLIC_HEADER "${headers}"
#)

install(
  TARGETS benchmark_main
  RUNTIME DESTINATION  bin
  ARCHIVE DESTINATION  lib
  LIBRARY DESTINATION lib
 )

 install (FILES ${BENCHMARK_HEADERS}
         DESTINATION include/bi_public/benchmark
          )
