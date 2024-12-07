# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fede/PCSC_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fede/PCSC_project

# Include any dependencies generated for this target.
include CMakeFiles/PCSC_fourier.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PCSC_fourier.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PCSC_fourier.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PCSC_fourier.dir/flags.make

CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o: CMakeFiles/PCSC_fourier.dir/flags.make
CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o: src/Fourier.cpp
CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o: CMakeFiles/PCSC_fourier.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fede/PCSC_project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o -MF CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o.d -o CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o -c /home/fede/PCSC_project/src/Fourier.cpp

CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fede/PCSC_project/src/Fourier.cpp > CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.i

CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fede/PCSC_project/src/Fourier.cpp -o CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.s

CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o: CMakeFiles/PCSC_fourier.dir/flags.make
CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o: src/Histogram.cpp
CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o: CMakeFiles/PCSC_fourier.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fede/PCSC_project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o -MF CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o.d -o CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o -c /home/fede/PCSC_project/src/Histogram.cpp

CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fede/PCSC_project/src/Histogram.cpp > CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.i

CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fede/PCSC_project/src/Histogram.cpp -o CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.s

CMakeFiles/PCSC_fourier.dir/src/main.cpp.o: CMakeFiles/PCSC_fourier.dir/flags.make
CMakeFiles/PCSC_fourier.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/PCSC_fourier.dir/src/main.cpp.o: CMakeFiles/PCSC_fourier.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fede/PCSC_project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/PCSC_fourier.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PCSC_fourier.dir/src/main.cpp.o -MF CMakeFiles/PCSC_fourier.dir/src/main.cpp.o.d -o CMakeFiles/PCSC_fourier.dir/src/main.cpp.o -c /home/fede/PCSC_project/src/main.cpp

CMakeFiles/PCSC_fourier.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PCSC_fourier.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fede/PCSC_project/src/main.cpp > CMakeFiles/PCSC_fourier.dir/src/main.cpp.i

CMakeFiles/PCSC_fourier.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PCSC_fourier.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fede/PCSC_project/src/main.cpp -o CMakeFiles/PCSC_fourier.dir/src/main.cpp.s

# Object files for target PCSC_fourier
PCSC_fourier_OBJECTS = \
"CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o" \
"CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o" \
"CMakeFiles/PCSC_fourier.dir/src/main.cpp.o"

# External object files for target PCSC_fourier
PCSC_fourier_EXTERNAL_OBJECTS =

build/PCSC_fourier: CMakeFiles/PCSC_fourier.dir/src/Fourier.cpp.o
build/PCSC_fourier: CMakeFiles/PCSC_fourier.dir/src/Histogram.cpp.o
build/PCSC_fourier: CMakeFiles/PCSC_fourier.dir/src/main.cpp.o
build/PCSC_fourier: CMakeFiles/PCSC_fourier.dir/build.make
build/PCSC_fourier: /usr/lib/x86_64-linux-gnu/libboost_iostreams.so.1.74.0
build/PCSC_fourier: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.74.0
build/PCSC_fourier: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.74.0
build/PCSC_fourier: CMakeFiles/PCSC_fourier.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fede/PCSC_project/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable build/PCSC_fourier"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PCSC_fourier.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PCSC_fourier.dir/build: build/PCSC_fourier
.PHONY : CMakeFiles/PCSC_fourier.dir/build

CMakeFiles/PCSC_fourier.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PCSC_fourier.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PCSC_fourier.dir/clean

CMakeFiles/PCSC_fourier.dir/depend:
	cd /home/fede/PCSC_project && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fede/PCSC_project /home/fede/PCSC_project /home/fede/PCSC_project /home/fede/PCSC_project /home/fede/PCSC_project/CMakeFiles/PCSC_fourier.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PCSC_fourier.dir/depend

