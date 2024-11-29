cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# Set debug flags for more control
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Specify the build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
message(${CMAKE_BINARY_DIR})
file(GLOB SOURCES "src/*.cpp")

include_directories(${CMAKE_BINARY_DIR}/eigen)
include_directories(/src/ext_libraries/stb_image_write.h)


# Add the executable
add_executable(PCSC_fourier ${SOURCES})

# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


# Link Boost libraries
target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem)

# Display a message after build
message(STATUS "Build files generated. Run 'cmake --build build' to compile.")