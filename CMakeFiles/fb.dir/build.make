# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/pi/EVD_Proj/test_fb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/EVD_Proj

# Include any dependencies generated for this target.
include CMakeFiles/fb.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fb.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fb.dir/flags.make

CMakeFiles/fb.dir/main.c.o: CMakeFiles/fb.dir/flags.make
CMakeFiles/fb.dir/main.c.o: test_fb/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/EVD_Proj/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/fb.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fb.dir/main.c.o   -c /home/pi/EVD_Proj/test_fb/main.c

CMakeFiles/fb.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fb.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/EVD_Proj/test_fb/main.c > CMakeFiles/fb.dir/main.c.i

CMakeFiles/fb.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fb.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/EVD_Proj/test_fb/main.c -o CMakeFiles/fb.dir/main.c.s

# Object files for target fb
fb_OBJECTS = \
"CMakeFiles/fb.dir/main.c.o"

# External object files for target fb
fb_EXTERNAL_OBJECTS =

fb: CMakeFiles/fb.dir/main.c.o
fb: CMakeFiles/fb.dir/build.make
fb: CMakeFiles/fb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/EVD_Proj/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable fb"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fb.dir/build: fb

.PHONY : CMakeFiles/fb.dir/build

CMakeFiles/fb.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fb.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fb.dir/clean

CMakeFiles/fb.dir/depend:
	cd /home/pi/EVD_Proj && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/EVD_Proj/test_fb /home/pi/EVD_Proj/test_fb /home/pi/EVD_Proj /home/pi/EVD_Proj /home/pi/EVD_Proj/CMakeFiles/fb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fb.dir/depend

