# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/RocketTracker/RocketTracker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/RocketTracker/RocketTracker

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ubuntu/RocketTracker/RocketTracker/CMakeFiles /home/ubuntu/RocketTracker/RocketTracker/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ubuntu/RocketTracker/RocketTracker/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named RocketTracker

# Build rule for target.
RocketTracker: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 RocketTracker
.PHONY : RocketTracker

# fast build rule for target.
RocketTracker/fast:
	$(MAKE) -f CMakeFiles/RocketTracker.dir/build.make CMakeFiles/RocketTracker.dir/build
.PHONY : RocketTracker/fast

Source.o: Source.cpp.o
.PHONY : Source.o

# target to build an object file
Source.cpp.o:
	$(MAKE) -f CMakeFiles/RocketTracker.dir/build.make CMakeFiles/RocketTracker.dir/Source.cpp.o
.PHONY : Source.cpp.o

Source.i: Source.cpp.i
.PHONY : Source.i

# target to preprocess a source file
Source.cpp.i:
	$(MAKE) -f CMakeFiles/RocketTracker.dir/build.make CMakeFiles/RocketTracker.dir/Source.cpp.i
.PHONY : Source.cpp.i

Source.s: Source.cpp.s
.PHONY : Source.s

# target to generate assembly for a file
Source.cpp.s:
	$(MAKE) -f CMakeFiles/RocketTracker.dir/build.make CMakeFiles/RocketTracker.dir/Source.cpp.s
.PHONY : Source.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... RocketTracker"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Source.o"
	@echo "... Source.i"
	@echo "... Source.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

