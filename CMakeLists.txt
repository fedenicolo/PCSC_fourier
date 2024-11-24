cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


# Add the executable
add_executable(PCSC_fourier main.cpp Histogram.cpp Visualizer.cpp Output.cpp Input.cpp BMPInput.cpp)

# Link Boost libraries
target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem)
