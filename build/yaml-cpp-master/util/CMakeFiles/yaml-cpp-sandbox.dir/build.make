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

# Include any dependencies generated for this target.
include yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/compiler_depend.make

# Include the progress variables for this target.
include yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/progress.make

# Include the compile flags for this target's objects.
include yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/flags.make

yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o: yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/flags.make
yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o: /Users/anatolii/Desktop/Oper_Syst/kp/yaml-cpp-master/util/sandbox.cpp
yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o: yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/anatolii/Desktop/Oper_Syst/kp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o"
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o -MF CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o.d -o CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o -c /Users/anatolii/Desktop/Oper_Syst/kp/yaml-cpp-master/util/sandbox.cpp

yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.i"
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/anatolii/Desktop/Oper_Syst/kp/yaml-cpp-master/util/sandbox.cpp > CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.i

yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.s"
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/anatolii/Desktop/Oper_Syst/kp/yaml-cpp-master/util/sandbox.cpp -o CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.s

# Object files for target yaml-cpp-sandbox
yaml__cpp__sandbox_OBJECTS = \
"CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o"

# External object files for target yaml-cpp-sandbox
yaml__cpp__sandbox_EXTERNAL_OBJECTS =

yaml-cpp-master/util/sandbox: yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/sandbox.cpp.o
yaml-cpp-master/util/sandbox: yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/build.make
yaml-cpp-master/util/sandbox: yaml-cpp-master/libyaml-cppd.a
yaml-cpp-master/util/sandbox: yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/anatolii/Desktop/Oper_Syst/kp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sandbox"
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yaml-cpp-sandbox.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/build: yaml-cpp-master/util/sandbox
.PHONY : yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/build

yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/clean:
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util && $(CMAKE_COMMAND) -P CMakeFiles/yaml-cpp-sandbox.dir/cmake_clean.cmake
.PHONY : yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/clean

yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/depend:
	cd /Users/anatolii/Desktop/Oper_Syst/kp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/anatolii/Desktop/Oper_Syst/kp /Users/anatolii/Desktop/Oper_Syst/kp/yaml-cpp-master/util /Users/anatolii/Desktop/Oper_Syst/kp/build /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util /Users/anatolii/Desktop/Oper_Syst/kp/build/yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : yaml-cpp-master/util/CMakeFiles/yaml-cpp-sandbox.dir/depend
