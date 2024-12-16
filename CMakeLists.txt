cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Set the build type to Debug
#set(CMAKE_BUILD_TYPE Debug)

option(DEBUG "Turn on or turn off debug build. Off by default" OFF)
if(DEBUG)
    message(STATUS "Debug build enabled")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")

else()
    message(STATUS "Release build enabled with O3 optimization")
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS "-O3")
endif()


# Specify the build output directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
message(STATUS "The CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}")
message(STATUS "The CMAKE_BINARY_DIR is ${CMAKE_BINARY_DIR}")


#file(GLOB MAIN_SOURCES "src/*.cpp")

set(GIT_EXECUTABLE "git")
option(GIT_SUBMODULE "Check submodules during build" ON)
if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
endif()
unset(GIT_SUBMODULE CACHE)

include_directories(${CMAKE_SOURCE_DIR}/eigen)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lame-3.100/include)
include_directories(${CMAKE_SOURCE_DIR}/mpg123-1.32.8/src/include)

# Add the executable 
add_executable(PCSC_fourier "${CMAKE_SOURCE_DIR}/src/Fourier.h" "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/Image.h" "${CMAKE_SOURCE_DIR}/src/Sound.h"
                            "${CMAKE_SOURCE_DIR}/src/WAVInput.cpp" "${CMAKE_SOURCE_DIR}/src/PNGInput.cpp" 
                            "${CMAKE_SOURCE_DIR}/src/BMPInput.cpp" "${CMAKE_SOURCE_DIR}/src/AudioExceptions.h" "${CMAKE_SOURCE_DIR}/src/ImageExceptions.h" 
                            "${CMAKE_SOURCE_DIR}/src/Bitmap.h" "${CMAKE_SOURCE_DIR}/src/main.cpp" "${CMAKE_SOURCE_DIR}/src/Visualizer.cpp"
                            "${CMAKE_SOURCE_DIR}/src/Histogram.cpp" "${CMAKE_SOURCE_DIR}/src/SignalProcessor.h" "${CMAKE_SOURCE_DIR}/src/PNGOutput.cpp" "${CMAKE_SOURCE_DIR}/src/CSVOutput.cpp"
                            "${CMAKE_SOURCE_DIR}/src/MP3Output.cpp" "${CMAKE_SOURCE_DIR}/src/Output.cpp")

# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
include_directories(${EIGEN3_INCLUDE_DIR})
# Link libraries
add_custom_target(build_mpg123 
                  COMMAND bash -c "./configure --prefix=${CMAKE_BINARY_DIR}/mpg123-1.32.8/local"
                  COMMAND bash -c "make"
                  COMMAND bash -c "make install"
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/mpg123-1.32.8
                  COMMENT "Building mpg123")

add_custom_target(build_lame
                  COMMAND bash -c "./configure --prefix=${CMAKE_BINARY_DIR}/lame-3.100"
                  COMMAND bash -c "make"
                  COMMAND bash -c "make install"
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lame-3.100
                  COMMENT "Building lame")
                  
                  
                  
add_dependencies(PCSC_fourier build_mpg123 build_lame)
target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)
link_directories(${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib)
link_directories(${CMAKE_BINARY_DIR}/lame-3.100/lib)


add_custom_target(main DEPENDS PCSC_fourier)


option(TESTS "Enable/Disable tests" OFF)
if(TESTS)
    message(STATUS "Tests Enabled")
    include_directories(${CMAKE_SOURCE_DIR}/googletest)
    add_subdirectory(${CMAKE_SOURCE_DIR}/googletest)
    
    add_executable(FourierTests "${CMAKE_SOURCE_DIR}/tests/FourierTests.cpp" "${CMAKE_SOURCE_DIR}/src/Fourier.h" "${CMAKE_SOURCE_DIR}/src/WAVInput.cpp" 
    "${CMAKE_SOURCE_DIR}/src/Sound.h" "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/Image.h" "${CMAKE_SOURCE_DIR}/src/BMPInput.cpp" "${CMAKE_SOURCE_DIR}/src/PNGInput.cpp" "${CMAKE_SOURCE_DIR}/src/AudioExceptions.h" "${CMAKE_SOURCE_DIR}/src/ImageExceptions.h" "${CMAKE_SOURCE_DIR}/src/Bitmap.h")
    
    add_executable(WAVInputTests "${CMAKE_SOURCE_DIR}/tests/WAVInputTests.cpp" "${CMAKE_SOURCE_DIR}/src/WAVInput.cpp" "${CMAKE_SOURCE_DIR}/src/Sound.h" 
    "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/AudioExceptions.h")
    
    add_executable(PNGInputTests "${CMAKE_SOURCE_DIR}/tests/PNGInputTests.cpp" "${CMAKE_SOURCE_DIR}/src/PNGInput.cpp" "${CMAKE_SOURCE_DIR}/src/Image.h" 
    "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/ImageExceptions.h")
    
    add_executable(BMPInputTests "${CMAKE_SOURCE_DIR}/tests/BMPInputTests.cpp" "${CMAKE_SOURCE_DIR}/src/BMPInput.cpp" "${CMAKE_SOURCE_DIR}/src/Image.h" 
    "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/ImageExceptions.h" "${CMAKE_SOURCE_DIR}/src/Bitmap.h")
    
    add_executable(VisualizerTests "${CMAKE_SOURCE_DIR}/tests/VisualizerTests.cpp" "${CMAKE_SOURCE_DIR}/src/WAVInput.cpp" "${CMAKE_SOURCE_DIR}/src/AudioExceptions.h" 
    "${CMAKE_SOURCE_DIR}/src/Visualizer.cpp" "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/Sound.h" "${CMAKE_SOURCE_DIR}/src/Histogram.cpp")
    
    add_executable(MP3OutputTests "${CMAKE_SOURCE_DIR}/tests/MP3OutputTests.cpp" "${CMAKE_SOURCE_DIR}/src/WAVInput.cpp" "${CMAKE_SOURCE_DIR}/src/MP3Output.cpp" "${CMAKE_SOURCE_DIR}/src/AudioExceptions.h" "${CMAKE_SOURCE_DIR}/src/Output.cpp" "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/Sound.h")
    
    add_executable(HistogramTests "${CMAKE_SOURCE_DIR}/tests/HistogramTests.cpp" "${CMAKE_SOURCE_DIR}/src/Histogram.cpp" "${CMAKE_SOURCE_DIR}/src/PNGInput.cpp" "${CMAKE_SOURCE_DIR}/src/Image.h" "${CMAKE_SOURCE_DIR}/src/Input.cpp")
    
    add_executable(FilterTests "${CMAKE_SOURCE_DIR}/tests/FilterTests.cpp" "${CMAKE_SOURCE_DIR}/src/WAVInput.cpp" "${CMAKE_SOURCE_DIR}/src/Fourier.h" "${CMAKE_SOURCE_DIR}/src/SignalProcessor.h" "${CMAKE_SOURCE_DIR}/src/Input.cpp" "${CMAKE_SOURCE_DIR}/src/Sound.h" "${CMAKE_SOURCE_DIR}/src/AudioExceptions.h")


    add_dependencies(FourierTests build_mpg123 build_lame)
    add_dependencies(MP3OutputTests build_mpg123 build_lame)

    target_link_libraries(FourierTests gtest gtest_main Boost::iostreams Boost::system Boost::filesystem pthread ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)
    target_link_libraries(WAVInputTests gtest gtest_main)
    target_link_libraries(PNGInputTests gtest gtest_main)
    target_link_libraries(BMPInputTests gtest gtest_main)
    target_link_libraries(VisualizerTests gtest gtest_main Boost::iostreams Boost::system Boost::filesystem )
    target_link_libraries(MP3OutputTests gtest gtest_main Boost::iostreams Boost::system Boost::filesystem ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)
    target_link_libraries(HistogramTests gtest gtest_main Boost::iostreams Boost::system Boost::filesystem)    
    target_link_libraries(FilterTests gtest gtest_main)

    add_custom_target(tests
        COMMAND bash -c "cp -rv ${CMAKE_SOURCE_DIR}/tests/Test_Files ${CMAKE_BINARY_DIR}"
        COMMAND bash -c "cp -v ${CMAKE_SOURCE_DIR}/tests/hash_check ${CMAKE_BINARY_DIR}"
        DEPENDS build_mpg123 build_lame FourierTests WAVInputTests PNGInputTests BMPInputTests VisualizerTests MP3OutputTests HistogramTests FilterTests
        COMMENT "Building all test executables"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    message("Build the tests by running `make tests`")

    add_custom_target(run_tests
        COMMAND bash -c "echo '########### Running Fourier Tests ##########'"
        COMMAND bash -c "./FourierTests"
        COMMAND bash -c "echo '########### Running WAV Tests     ##########'"
        COMMAND bash -c "./WAVInputTests"
        COMMAND bash -c "echo '########### Running PNG Tests     ##########'"
        COMMAND bash -c "./PNGInputTests"
        COMMAND bash -c "echo '########### Running BMP Tests     ##########'"
        COMMAND bash -c "./BMPInputTests"
        COMMAND bash -c "echo '######## Running Visualizer Tests ##########'"
        COMMAND bash -c "./VisualizerTests"
        COMMAND bash -c "./hash_check Test_Files/hist_control_sine24b.png hist_sine24b.png"
        COMMAND bash -c "./hash_check Test_Files/line_control_sine24b.png line_sine24b.png"
        COMMAND bash -c "echo '######### Running MP3Output Tests ##########'"
        COMMAND bash -c "./MP3OutputTests"
        COMMAND bash -c "./hash_check Test_Files/Sine16bit_control.mp3 Sine16bitOutput.mp3"
        COMMAND bash -c "echo '######### Running Histogram Tests ##########'"
        COMMAND bash -c "./HistogramTests"
        COMMAND bash -c "echo '######### Running Filter Tests ##########'"
        COMMAND bash -c "./FilterTests"
        COMMAND bash -c "./hash_check Test_Files/lowpass.txt lowpass.txt"
        COMMAND bash -c "./hash_check Test_Files/highpass.txt highpass.txt"
        COMMAND bash -c "rm -rv Test_Files/ hist_sine24b.png line_sine24b.png Sine16bitOutput.mp3 lowpass.txt highpass.txt"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running all tests"
    )

    message(STATUS "Run `make run_tests` to run the tests")
endif(TESTS)

option(DOCUMENTATION "Activate documentation" OFF)
if (DOCUMENTATION)
   set(DOXYGEN_EXECUTABLE doxygen)
   set(DOXYFILE Doxyfile)
   add_custom_target(doc_doxygen ALL
     COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${DOXYFILE}
     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
     COMMENT "Generating API documentation with Doxygen"
     VERBATIM)
endif(DOCUMENTATION)
unset(DOCUMENTATION CACHE)

# Display a message after build
message(STATUS "Build files generated. Run make to compile")