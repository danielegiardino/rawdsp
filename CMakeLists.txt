cmake_minimum_required(VERSION 3.10)

project(rawdsp LANGUAGES CXX DESCRIPTION "DSP Kit")
message("PROJECT_NAME: " ${PROJECT_NAME})

# Set compilers
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

################################################################
######## Auto-vectorization
add_compile_options(-march=x86-64 -mtune=generic)
# add_compile_options(-O3 -msse2 -msse -m64 -mmmx -mavx)
add_compile_options(-O3)
add_compile_options(-m64)
add_compile_options(-msse)
add_compile_options(-msse2)
add_compile_options(-mmmx)
add_compile_options(-mfma)
add_compile_options(-mavx)
add_compile_options(-mavx2)
add_compile_options(-ftree-vectorize)
add_compile_options(-funroll-loops)
add_compile_options(-ffast-math)
add_compile_options(-ftree-loop-distribution)

######## Auto-Parallelization
add_compile_options(-floop-parallelize-all)

################################################################
# Info
# add_compile_options(-fopt-info-vec-optimized)
# add_compile_options(-fopt-info-vec)
# add_compile_options(-fopt-info-omp)
# add_compile_options(-fopt-info-loop)

# OpenMP for parallelization
find_package(OpenMP REQUIRED)

# FFTW3
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3)

# Enable pthread for affinity and threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(lib/include)
include_directories(include)

# Function to create executables for each test/example file
function(create_executable_for_each_file DIR)
    file(GLOB FILES "${DIR}/*.cpp")
    foreach(FILE ${FILES})
        get_filename_component(FILENAME ${FILE} NAME_WE)
        add_executable(${FILENAME} ${FILE})
        target_link_libraries(${FILENAME} 
            PRIVATE Threads::Threads
            PRIVATE OpenMP::OpenMP_CXX
            PRIVATE ${FFTW3_LIBRARIES})
    endforeach()
endfunction()

# Create executables for test files
create_executable_for_each_file("tests")

# Create executables for example files
create_executable_for_each_file("examples")
