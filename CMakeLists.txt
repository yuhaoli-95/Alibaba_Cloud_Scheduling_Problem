cmake_minimum_required(VERSION 3.0.0)
project(tianchi VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_STANDARD 11)
add_executable(tianchi 
./code/main.cpp
./code/input_data.cpp
./code/check_file_encoding.cpp
./code/tools.cpp
./code/get_bounds.cpp
./code/iniltialization.cpp
./code/optimize_sigma.cpp

./code/evaluation_model/process_task.cpp
./code/evaluation_model/fitness.cpp
./code/evaluation_model/calculate_score.cpp

./code/multi_local_search/local_search.cpp
./code/multi_local_search/k_opt_mutation.cpp
./code/multi_local_search/k_opt_swap.cpp
./code/multi_local_search/is_find_better.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


cmake_minimum_required(VERSION 3.0)

project(TaskDispatcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
    COMMAND
        cp ${CMAKE_CURRENT_SOURCE_DIR}/process_time_matrix.csv ${CMAKE_CURRENT_BINARY_DIR}/process_time_matrix.csv
    COMMAND
        cp ${CMAKE_CURRENT_SOURCE_DIR}/work_order.csv ${CMAKE_CURRENT_BINARY_DIR}/work_order.csv
    )


add_executable(scorer ./code/scorer.cpp)