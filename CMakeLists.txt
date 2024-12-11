cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Set the build type to Debug
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

# Set debug flags for more control
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")

set(CMAKE_CXX_FLAGS "-O3")

# Specify the build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
message(${CMAKE_BINARY_DIR})
file(GLOB MAIN_SOURCES "src/*.cpp")

include_directories(${CMAKE_BINARY_DIR}/eigen)
include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/googletest)
include_directories(/src/stb_image_write.h)
include_directories(${CMAKE_BINARY_DIR}/lame-3.100/include)
include_directories(${CMAKE_BINARY_DIR}/mpg123-1.32.8/src/include)
add_subdirectory(${CMAKE_BINARY_DIR}/googletest)

# Add the executable 
add_executable(PCSC_fourier ${MAIN_SOURCES})
set(TEST_SRC "tests/FourierTests.cpp" "src/Fourier.h" "src/WAVInput.cpp" "src/Sound.h" "src/Input.cpp" "src/Image.h" "src/BMPInput.cpp")
add_executable(FourierTests ${TEST_SRC})


# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
include_directories(${EIGEN3_INCLUDE_DIR})

# Link Boost libraries
link_directories(${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib)
link_directories($(CMAKE_BINARY_DIR)/lame-3.100/lib)

target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)
target_link_libraries(FourierTests gtest gtest_main Boost::iostreams Boost::system Boost::filesystem pthread ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)



# Display a message after build
message(STATUS "Build files generated. Run make to compile")