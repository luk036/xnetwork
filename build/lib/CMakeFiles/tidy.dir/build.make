# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lubuntu/github/luk036/xnetwork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lubuntu/github/luk036/xnetwork/build

# Utility rule file for tidy.

# Include the progress variables for this target.
include lib/CMakeFiles/tidy.dir/progress.make

lib/CMakeFiles/tidy:
	cd /home/lubuntu/github/luk036/xnetwork/build/lib && /media/lubuntu/USBDISK/miniconda3/bin/clang-tidy -header-filter=.* /home/lubuntu/github/luk036/xnetwork/lib/bench/one_example.cpp /home/lubuntu/github/luk036/xnetwork/lib/src/xnetwork.cpp /home/lubuntu/github/luk036/xnetwork/lib/test/src/test_main.cpp /home/lubuntu/github/luk036/xnetwork/lib/test/src/test_xnetwork.cpp -p=./

tidy: lib/CMakeFiles/tidy
tidy: lib/CMakeFiles/tidy.dir/build.make

.PHONY : tidy

# Rule to build all files generated by this target.
lib/CMakeFiles/tidy.dir/build: tidy

.PHONY : lib/CMakeFiles/tidy.dir/build

lib/CMakeFiles/tidy.dir/clean:
	cd /home/lubuntu/github/luk036/xnetwork/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/tidy.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/tidy.dir/clean

lib/CMakeFiles/tidy.dir/depend:
	cd /home/lubuntu/github/luk036/xnetwork/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lubuntu/github/luk036/xnetwork /home/lubuntu/github/luk036/xnetwork/lib /home/lubuntu/github/luk036/xnetwork/build /home/lubuntu/github/luk036/xnetwork/build/lib /home/lubuntu/github/luk036/xnetwork/build/lib/CMakeFiles/tidy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/tidy.dir/depend

