cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Specify the build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
message(${CMAKE_BINARY_DIR})
file(GLOB SOURCES "src/*.cpp")

include_directories(${CMAKE_BINARY_DIR}/eigen)

# Add the executable
add_executable(PCSC_fourier ${SOURCES})

# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)

# Link Boost libraries
target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem)

# Display a message after build
message(STATUS "Build files generated. Run 'cmake --build build' to compile.")