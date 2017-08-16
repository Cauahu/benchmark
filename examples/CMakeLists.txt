include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/examples)

add_executable(example
  rider_benchmark.cc
  rider_benchmark_with_fixture.cc
  rider_benchmark_parameterized.cc
  rider_benchmark_parameterized_with_fixture.cc
  rider_sleep.cc
)

target_link_libraries(example
  benchmark_main
)
