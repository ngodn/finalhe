# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92

# Include any dependencies generated for this target.
include deps/miniz/CMakeFiles/miniz.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include deps/miniz/CMakeFiles/miniz.dir/compiler_depend.make

# Include the progress variables for this target.
include deps/miniz/CMakeFiles/miniz.dir/progress.make

# Include the compile flags for this target's objects.
include deps/miniz/CMakeFiles/miniz.dir/flags.make

deps/miniz/CMakeFiles/miniz.dir/miniz.c.o: deps/miniz/CMakeFiles/miniz.dir/flags.make
deps/miniz/CMakeFiles/miniz.dir/miniz.c.o: deps/miniz/miniz.c
deps/miniz/CMakeFiles/miniz.dir/miniz.c.o: deps/miniz/CMakeFiles/miniz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object deps/miniz/CMakeFiles/miniz.dir/miniz.c.o"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT deps/miniz/CMakeFiles/miniz.dir/miniz.c.o -MF CMakeFiles/miniz.dir/miniz.c.o.d -o CMakeFiles/miniz.dir/miniz.c.o -c /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz.c

deps/miniz/CMakeFiles/miniz.dir/miniz.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniz.dir/miniz.c.i"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz.c > CMakeFiles/miniz.dir/miniz.c.i

deps/miniz/CMakeFiles/miniz.dir/miniz.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniz.dir/miniz.c.s"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz.c -o CMakeFiles/miniz.dir/miniz.c.s

deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.o: deps/miniz/CMakeFiles/miniz.dir/flags.make
deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.o: deps/miniz/miniz_zip.c
deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.o: deps/miniz/CMakeFiles/miniz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.o"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.o -MF CMakeFiles/miniz.dir/miniz_zip.c.o.d -o CMakeFiles/miniz.dir/miniz_zip.c.o -c /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_zip.c

deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniz.dir/miniz_zip.c.i"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_zip.c > CMakeFiles/miniz.dir/miniz_zip.c.i

deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniz.dir/miniz_zip.c.s"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_zip.c -o CMakeFiles/miniz.dir/miniz_zip.c.s

deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.o: deps/miniz/CMakeFiles/miniz.dir/flags.make
deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.o: deps/miniz/miniz_tinfl.c
deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.o: deps/miniz/CMakeFiles/miniz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.o"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.o -MF CMakeFiles/miniz.dir/miniz_tinfl.c.o.d -o CMakeFiles/miniz.dir/miniz_tinfl.c.o -c /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_tinfl.c

deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniz.dir/miniz_tinfl.c.i"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_tinfl.c > CMakeFiles/miniz.dir/miniz_tinfl.c.i

deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniz.dir/miniz_tinfl.c.s"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_tinfl.c -o CMakeFiles/miniz.dir/miniz_tinfl.c.s

deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.o: deps/miniz/CMakeFiles/miniz.dir/flags.make
deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.o: deps/miniz/miniz_tdef.c
deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.o: deps/miniz/CMakeFiles/miniz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.o"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.o -MF CMakeFiles/miniz.dir/miniz_tdef.c.o.d -o CMakeFiles/miniz.dir/miniz_tdef.c.o -c /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_tdef.c

deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/miniz.dir/miniz_tdef.c.i"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_tdef.c > CMakeFiles/miniz.dir/miniz_tdef.c.i

deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/miniz.dir/miniz_tdef.c.s"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/miniz_tdef.c -o CMakeFiles/miniz.dir/miniz_tdef.c.s

# Object files for target miniz
miniz_OBJECTS = \
"CMakeFiles/miniz.dir/miniz.c.o" \
"CMakeFiles/miniz.dir/miniz_zip.c.o" \
"CMakeFiles/miniz.dir/miniz_tinfl.c.o" \
"CMakeFiles/miniz.dir/miniz_tdef.c.o"

# External object files for target miniz
miniz_EXTERNAL_OBJECTS =

deps/miniz/libminiz.a: deps/miniz/CMakeFiles/miniz.dir/miniz.c.o
deps/miniz/libminiz.a: deps/miniz/CMakeFiles/miniz.dir/miniz_zip.c.o
deps/miniz/libminiz.a: deps/miniz/CMakeFiles/miniz.dir/miniz_tinfl.c.o
deps/miniz/libminiz.a: deps/miniz/CMakeFiles/miniz.dir/miniz_tdef.c.o
deps/miniz/libminiz.a: deps/miniz/CMakeFiles/miniz.dir/build.make
deps/miniz/libminiz.a: deps/miniz/CMakeFiles/miniz.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C static library libminiz.a"
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && $(CMAKE_COMMAND) -P CMakeFiles/miniz.dir/cmake_clean_target.cmake
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/miniz.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
deps/miniz/CMakeFiles/miniz.dir/build: deps/miniz/libminiz.a
.PHONY : deps/miniz/CMakeFiles/miniz.dir/build

deps/miniz/CMakeFiles/miniz.dir/clean:
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz && $(CMAKE_COMMAND) -P CMakeFiles/miniz.dir/cmake_clean.cmake
.PHONY : deps/miniz/CMakeFiles/miniz.dir/clean

deps/miniz/CMakeFiles/miniz.dir/depend:
	cd /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92 /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92 /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz /Users/dennyaprilio/Documents/psvita/jailbreak/FinalHE_1-92_and_Dependencies/finalhe-1.92/deps/miniz/CMakeFiles/miniz.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : deps/miniz/CMakeFiles/miniz.dir/depend

