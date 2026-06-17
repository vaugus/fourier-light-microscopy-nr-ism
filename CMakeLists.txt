cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------
# OpenMP
# -------------------------
find_package(OpenMP REQUIRED)

# -------------------------
# OpenCV
# -------------------------
find_package(OpenCV REQUIRED)

# -------------------------
# Include directories
# -------------------------
include_directories(
    lib/include
)

# -------------------------
# Source files
# -------------------------
set(SOURCES
    src/main.cpp
    lib/src/csv.cpp
    lib/src/helper.cpp
    lib/src/fft_utils.cpp
    lib/src/sample_function_base.cpp
    lib/src/statistical_analysis.cpp
    lib/src/sample_function.cpp
)

# -------------------------
# Executable
# -------------------------
add_executable(main ${SOURCES})

# -------------------------
# Link libraries
# -------------------------
target_link_libraries(main
    PRIVATE
        OpenMP::OpenMP_CXX
        ${OpenCV_LIBS}
)

# -------------------------
# Compiler flags (optional extra tuning)
# -------------------------
target_compile_options(main PRIVATE -Wall)