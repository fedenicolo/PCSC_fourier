cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)

# Add the executable
add_executable(PCSC_fourier main.cpp Histogram.cpp Visualizer.cpp Output.cpp)

# Link Boost libraries
target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem)
