# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/anatolii/Desktop/Oper_Syst/kp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/anatolii/Desktop/Oper_Syst/kp/build

# Utility rule file for ContinuousSubmit.

# Include any custom commands dependencies for this target.
include yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/compiler_depend.make

# Include the progress variables for this target.
include yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/progress.make

yaml-cpp-master/CMakeFiles/ContinuousSubmit:
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master && /opt/homebrew/Cellar/cmake/3.27.4/bin/ctest -D ContinuousSubmit

ContinuousSubmit: yaml-cpp-master/CMakeFiles/ContinuousSubmit
ContinuousSubmit: yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/build.make
.PHONY : ContinuousSubmit

# Rule to build all files generated by this target.
yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/build: ContinuousSubmit
.PHONY : yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/build

yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/clean:
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousSubmit.dir/cmake_clean.cmake
.PHONY : yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/clean

yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/depend:
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/anatolii/Desktop/Oper_Syst/kp /Users/anatolii/Desktop/Oper_Syst/kp/yaml-cpp-master /Users/anatolii/Desktop/Oper_Syst/kp/build /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : yaml-cpp-master/CMakeFiles/ContinuousSubmit.dir/depend

