# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/nxirda/Desktop/PPN_Projet_MEM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nxirda/Desktop/PPN_Projet_MEM

# Include any dependencies generated for this target.
include test/CMakeFiles/Test_Lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/Test_Lib.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/Test_Lib.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/Test_Lib.dir/flags.make

test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o: test/CMakeFiles/Test_Lib.dir/flags.make
test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o: test/DataSet_Test.cpp
test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o: test/CMakeFiles/Test_Lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nxirda/Desktop/PPN_Projet_MEM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o -MF CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o.d -o CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o -c /home/nxirda/Desktop/PPN_Projet_MEM/test/DataSet_Test.cpp

test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.i"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nxirda/Desktop/PPN_Projet_MEM/test/DataSet_Test.cpp > CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.i

test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.s"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nxirda/Desktop/PPN_Projet_MEM/test/DataSet_Test.cpp -o CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.s

test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o: test/CMakeFiles/Test_Lib.dir/flags.make
test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o: test/DecisionTree_Test.cpp
test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o: test/CMakeFiles/Test_Lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nxirda/Desktop/PPN_Projet_MEM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o -MF CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o.d -o CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o -c /home/nxirda/Desktop/PPN_Projet_MEM/test/DecisionTree_Test.cpp

test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.i"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nxirda/Desktop/PPN_Projet_MEM/test/DecisionTree_Test.cpp > CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.i

test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.s"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nxirda/Desktop/PPN_Projet_MEM/test/DecisionTree_Test.cpp -o CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.s

test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o: test/CMakeFiles/Test_Lib.dir/flags.make
test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o: test/TreeNode_Test.cpp
test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o: test/CMakeFiles/Test_Lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nxirda/Desktop/PPN_Projet_MEM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o -MF CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o.d -o CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o -c /home/nxirda/Desktop/PPN_Projet_MEM/test/TreeNode_Test.cpp

test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.i"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nxirda/Desktop/PPN_Projet_MEM/test/TreeNode_Test.cpp > CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.i

test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.s"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && /home/nxirda/tmp/mpicxx/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nxirda/Desktop/PPN_Projet_MEM/test/TreeNode_Test.cpp -o CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.s

# Object files for target Test_Lib
Test_Lib_OBJECTS = \
"CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o" \
"CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o" \
"CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o"

# External object files for target Test_Lib
Test_Lib_EXTERNAL_OBJECTS =

test/libTest_Lib.a: test/CMakeFiles/Test_Lib.dir/DataSet_Test.cpp.o
test/libTest_Lib.a: test/CMakeFiles/Test_Lib.dir/DecisionTree_Test.cpp.o
test/libTest_Lib.a: test/CMakeFiles/Test_Lib.dir/TreeNode_Test.cpp.o
test/libTest_Lib.a: test/CMakeFiles/Test_Lib.dir/build.make
test/libTest_Lib.a: test/CMakeFiles/Test_Lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nxirda/Desktop/PPN_Projet_MEM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libTest_Lib.a"
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && $(CMAKE_COMMAND) -P CMakeFiles/Test_Lib.dir/cmake_clean_target.cmake
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Test_Lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/Test_Lib.dir/build: test/libTest_Lib.a
.PHONY : test/CMakeFiles/Test_Lib.dir/build

test/CMakeFiles/Test_Lib.dir/clean:
	cd /home/nxirda/Desktop/PPN_Projet_MEM/test && $(CMAKE_COMMAND) -P CMakeFiles/Test_Lib.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/Test_Lib.dir/clean

test/CMakeFiles/Test_Lib.dir/depend:
	cd /home/nxirda/Desktop/PPN_Projet_MEM && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nxirda/Desktop/PPN_Projet_MEM /home/nxirda/Desktop/PPN_Projet_MEM/test /home/nxirda/Desktop/PPN_Projet_MEM /home/nxirda/Desktop/PPN_Projet_MEM/test /home/nxirda/Desktop/PPN_Projet_MEM/test/CMakeFiles/Test_Lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/Test_Lib.dir/depend

