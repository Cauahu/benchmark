#build benchmark lib and example binary by huzhuang.


cc_library(
    name = 'benchmark_main',
    optimize = ['O2', 'std=c++11',
        'Wall',
        'Wextra',
        'Werror',
        'Wno-unused-parameter',
        'Woverloaded-virtual',
        'Wpointer-arith',
        'Wshadow',
        'Wwrite-strings',
        'MMD', 
    ],
    srcs = [
        'src/benchmark/benchmark_main.cc',
    ],
   
    incs = [
       'include',
    ],
    export_incs = [
        'include',
    ],
)


cc_binary(
    name = 'example',
    optimize = ['O2', 'std=c++11',
       'Wall',
       'Wextra',
       'Werror',
       'Wno-unused-parameter',
       'Woverloaded-virtual',
       'Wpointer-arith',
       'Wshadow',
       'Wwrite-strings',
       'MMD', 
   ],
   srcs = [
        'examples/rider_benchmark.cc',
# 'examples/rider_benchmark_parameterized.cc',
#       'examples/rider_benchmark_parameterized_with_fixture.cc',
#       'examples/rider_benchmark_with_fixture.cc',
        'examples/rider_sleep.cc',
   ],
   incs = [
       'examples',
       'include',
   ],

   deps = [
        ':benchmark_main',    
   ],
)
