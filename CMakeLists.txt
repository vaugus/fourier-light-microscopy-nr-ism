cmake_minimum_required(VERSION 3.10)
project(ftiqa LANGUAGES CXX)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

add_library(ftiqa
    src/ftiqa/csv.cpp
    src/ftiqa/fft_utils.cpp
    src/ftiqa/helper.cpp
    src/ftiqa/image_util.cpp
    src/ftiqa/sample_function.cpp
    src/ftiqa/sample_function_base.cpp
    src/ftiqa/statistical_analysis.cpp
)

target_compile_features(ftiqa PUBLIC cxx_std_17)

target_include_directories(ftiqa
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(ftiqa
    PUBLIC
        ${OpenCV_LIBS}
    PRIVATE
        OpenMP::OpenMP_CXX
)

target_compile_options(ftiqa PRIVATE -Wall)

add_executable(main app/main.cpp)

target_link_libraries(main
    PRIVATE
        ftiqa
)
