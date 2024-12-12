cmake_minimum_required(VERSION 3.22)
project(PCSC_fourier)

set(CMAKE_CXX_STANDARD 20)

# Set the build type to Debug
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Debug)

# Set debug flags for more control
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_FLAGS "-O3")

# Specify the build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
message(${CMAKE_BINARY_DIR})
file(GLOB MAIN_SOURCES "src/*.cpp")

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

include_directories(${CMAKE_BINARY_DIR}/eigen)
include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(/src/stb_image_write.h)
include_directories(${CMAKE_BINARY_DIR}/lame-3.100/include)
include_directories(${CMAKE_BINARY_DIR}/mpg123-1.32.8/src/include)

# Add the executable 
add_executable(PCSC_fourier "src/Fourier.h" "src/Input.cpp" "src/Image.h" "src/Sound.h"
                            "src/WAVInput.cpp" "src/PNGInput.cpp" 
                            "src/BMPInput.cpp" "src/AudioExceptions.h" "src/ImageExceptions.h" 
                            "src/Bitmap.h" "src/main.cpp" "src/Visualizer.cpp"
                            "src/Histogram.cpp" "src/SignalProcessor.h" "src/PNGOutput.cpp" "src/CSVOutput.cpp"
                            "src/MP3Output.cpp" "src/Output.cpp")

# Find Boost components
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
include_directories(${EIGEN3_INCLUDE_DIR})
# Link libraries
link_directories(${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib)
link_directories($(CMAKE_BINARY_DIR)/lame-3.100/lib)

target_link_libraries(PCSC_fourier Boost::iostreams Boost::system Boost::filesystem ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)


add_custom_target(main DEPENDS PCSC_fourier)

option(TESTS "Enable/Disable tests" OFF)
if(TESTS)
    include_directories(${CMAKE_BINARY_DIR}/googletest)
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest)
    
    add_executable(FourierTests "tests/FourierTests.cpp" "src/Fourier.h" "src/WAVInput.cpp" 
    "src/Sound.h" "src/Input.cpp" "src/Image.h" "src/BMPInput.cpp" "src/PNGInput.cpp" "src/AudioExceptions.h" "src/ImageExceptions.h" "src/Bitmap.h")
    
    add_executable(WAVInputTests "tests/WAVInputTests.cpp" "src/WAVInput.cpp" "src/Sound.h" 
    "src/Input.cpp" "src/AudioExceptions.h")
    
    add_executable(PNGInputTests "tests/PNGInputTests.cpp" "src/PNGInput.cpp" "src/Image.h" 
    "src/Input.cpp" "src/ImageExceptions.h")
    
    add_executable(BMPInputTests "tests/BMPInputTests.cpp" "src/BMPInput.cpp" "src/Image.h" 
    "src/Input.cpp" "src/ImageExceptions.h" "src/Bitmap.h")
    
    add_executable(VisualizerTests "tests/VisualizerTests.cpp" "src/WAVInput.cpp" "src/AudioExceptions.h" 
    "src/Visualizer.cpp" "src/Input.cpp" "src/Sound.h" "src/Histogram.cpp")
    
    add_executable(MP3OutputTests "tests/MP3OutputTests.cpp" "src/WAVInput.cpp" "src/MP3Output.cpp" "src/AudioExceptions.h" "src/Output.cpp" "src/Input.cpp" "src/Sound.h")
    
    add_executable(HistogramTests "tests/HistogramTests.cpp" "src/Histogram.cpp" "src/PNGInput.cpp" "src/Image.h" "src/Input.cpp")
    
    target_link_libraries(FourierTests gtest gtest_main Boost::iostreams Boost::system Boost::filesystem pthread ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)
    target_link_libraries(WAVInputTests gtest gtest_main)
    target_link_libraries(PNGInputTests gtest gtest_main)
    target_link_libraries(BMPInputTests gtest gtest_main)
    target_link_libraries(VisualizerTests gtest Boost::iostreams Boost::system Boost::filesystem )
    target_link_libraries(MP3OutputTests gtest Boost::iostreams Boost::system Boost::filesystem ${CMAKE_BINARY_DIR}/mpg123-1.32.8/local/lib/libmpg123.so ${CMAKE_BINARY_DIR}/lame-3.100/lib/libmp3lame.so)
    target_link_libraries(HistogramTests gtest Boost::iostreams Boost::system Boost::filesystem)    


    add_custom_target(tests
        COMMAND bash -c "cp -rv ${CMAKE_BINARY_DIR}/tests/Test_Files ${CMAKE_BINARY_DIR}/build"
        DEPENDS FourierTests WAVInputTests PNGInputTests BMPInputTests VisualizerTests MP3OutputTests HistogramTests
        COMMENT "Building all test executables"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    add_custom_target(run_tests COMMAND bash -c "./FourierTests"
                                COMMAND bash -c "./WAVInputTests"
                                COMMAND bash -c "./PNGInputTests"
                                COMMAND bash -c "./BMPInputTests"
                                COMMAND bash -c "./VisualizerTests"
                                COMMAND bash -c "./MP3OutputTests"
                                COMMAND bash -c "
                                    hash1=\$(sha1sum Test_Files/Sine16bit_control.mp3 | awk '{print \$1}');
                                    hash2=\$(sha1sum ./Sine16bitOutput.mp3 | awk '{print \$1}');
                                    if [ \"\$hash1\" = \"\$hash2\" ]; then
                                        echo 'The hash of the MP3 output MATCHES the control.';
                                        exit 0;
                                    else
                                        echo 'The hash of the MP3 output does NOT MATCH the control.';
                                        exit 1;
                                    fi" 
                                COMMAND bash -c "./HistogramTests"
                                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build)

    message(STATUS "Run `make run_tests` to run the tests")
endif(TESTS)
unset(TESTS CACHE)

option(DOCUMENTATION "Activate documentation" OFF)
if (DOCUMENTATION)
   set(DOXYGEN_EXECUTABLE doxygen)
   set(DOXYFILE Doxyfile)
   add_custom_target(doc_doxygen ALL
     COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${DOXYFILE}
     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
     COMMENT "Generating API documentation with Doxygen"
     VERBATIM)
endif(DOCUMENTATION)
unset(DOCUMENTATION CACHE)

# Display a message after build
message(STATUS "Build files generated. Run make to compile")