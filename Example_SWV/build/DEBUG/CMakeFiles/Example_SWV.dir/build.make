# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV/build/DEBUG

# Utility rule file for Example_SWV.

# Include any custom commands dependencies for this target.
include CMakeFiles/Example_SWV.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Example_SWV.dir/progress.make

CMakeFiles/Example_SWV: Example_SWV.elf
	/Volumes/Data/VScode/GIT/VSCode_OnethinxPack_macOS/tools_3.0/cymcuelftool-1.0/bin/cymcuelftool --sign Example_SWV.elf --output Example_SWV_signed.elf --hex Example_SWV.hex
	/opt/local/bin/cmake -E copy Example_SWV.elf ../Example_SWV.elf
	/opt/local/bin/cmake -E copy compile_commands.json ../compile_commands.json
	echo -------\ Memory\ usage\ -------
	/Volumes/Data/VScode/GIT/VSCode_OnethinxPack_macOS/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-readelf -Sl Example_SWV.elf > Example_SWV.readelf
	/bin/bash /Volumes/Data/VScode/GIT/VSCode_OnethinxPack_macOS/config/scripts/memcalc.bash Example_SWV.readelf 917504 229376 268435456 134217728

Example_SWV: CMakeFiles/Example_SWV
Example_SWV: CMakeFiles/Example_SWV.dir/build.make
.PHONY : Example_SWV

# Rule to build all files generated by this target.
CMakeFiles/Example_SWV.dir/build: Example_SWV
.PHONY : CMakeFiles/Example_SWV.dir/build

CMakeFiles/Example_SWV.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Example_SWV.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Example_SWV.dir/clean

CMakeFiles/Example_SWV.dir/depend:
	cd /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV/build/DEBUG && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV/build/DEBUG /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV/build/DEBUG /Volumes/Data/VScode/GIT/Onethinx_Project_Examples/Example_SWV/build/DEBUG/CMakeFiles/Example_SWV.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Example_SWV.dir/depend
