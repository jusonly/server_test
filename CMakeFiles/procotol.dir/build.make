# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /workspace/NanTongProtocol2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspace/NanTongProtocol2

# Include any dependencies generated for this target.
include CMakeFiles/procotol.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/procotol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/procotol.dir/flags.make

CMakeFiles/procotol.dir/main.cpp.o: CMakeFiles/procotol.dir/flags.make
CMakeFiles/procotol.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/procotol.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/procotol.dir/main.cpp.o -c /workspace/NanTongProtocol2/main.cpp

CMakeFiles/procotol.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/procotol.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/NanTongProtocol2/main.cpp > CMakeFiles/procotol.dir/main.cpp.i

CMakeFiles/procotol.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/procotol.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/NanTongProtocol2/main.cpp -o CMakeFiles/procotol.dir/main.cpp.s

CMakeFiles/procotol.dir/protocol/protocol.c.o: CMakeFiles/procotol.dir/flags.make
CMakeFiles/procotol.dir/protocol/protocol.c.o: protocol/protocol.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/procotol.dir/protocol/protocol.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/procotol.dir/protocol/protocol.c.o -c /workspace/NanTongProtocol2/protocol/protocol.c

CMakeFiles/procotol.dir/protocol/protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/procotol.dir/protocol/protocol.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/NanTongProtocol2/protocol/protocol.c > CMakeFiles/procotol.dir/protocol/protocol.c.i

CMakeFiles/procotol.dir/protocol/protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/procotol.dir/protocol/protocol.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/NanTongProtocol2/protocol/protocol.c -o CMakeFiles/procotol.dir/protocol/protocol.c.s

CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o: CMakeFiles/procotol.dir/flags.make
CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o: protocol/protocol_client_proc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o -c /workspace/NanTongProtocol2/protocol/protocol_client_proc.c

CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/NanTongProtocol2/protocol/protocol_client_proc.c > CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.i

CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/NanTongProtocol2/protocol/protocol_client_proc.c -o CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.s

CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o: CMakeFiles/procotol.dir/flags.make
CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o: protocol/protocol_prco_abstruct.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o -c /workspace/NanTongProtocol2/protocol/protocol_prco_abstruct.c

CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/NanTongProtocol2/protocol/protocol_prco_abstruct.c > CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.i

CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/NanTongProtocol2/protocol/protocol_prco_abstruct.c -o CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.s

CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.o: CMakeFiles/procotol.dir/flags.make
CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.o: protocol/protocol_server_proc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.o -c /workspace/NanTongProtocol2/protocol/protocol_server_proc.cpp

CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspace/NanTongProtocol2/protocol/protocol_server_proc.cpp > CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.i

CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspace/NanTongProtocol2/protocol/protocol_server_proc.cpp -o CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.s

CMakeFiles/procotol.dir/utils/mini_utils.c.o: CMakeFiles/procotol.dir/flags.make
CMakeFiles/procotol.dir/utils/mini_utils.c.o: utils/mini_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/procotol.dir/utils/mini_utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/procotol.dir/utils/mini_utils.c.o -c /workspace/NanTongProtocol2/utils/mini_utils.c

CMakeFiles/procotol.dir/utils/mini_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/procotol.dir/utils/mini_utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /workspace/NanTongProtocol2/utils/mini_utils.c > CMakeFiles/procotol.dir/utils/mini_utils.c.i

CMakeFiles/procotol.dir/utils/mini_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/procotol.dir/utils/mini_utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /workspace/NanTongProtocol2/utils/mini_utils.c -o CMakeFiles/procotol.dir/utils/mini_utils.c.s

# Object files for target procotol
procotol_OBJECTS = \
"CMakeFiles/procotol.dir/main.cpp.o" \
"CMakeFiles/procotol.dir/protocol/protocol.c.o" \
"CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o" \
"CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o" \
"CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.o" \
"CMakeFiles/procotol.dir/utils/mini_utils.c.o"

# External object files for target procotol
procotol_EXTERNAL_OBJECTS =

bin/procotol: CMakeFiles/procotol.dir/main.cpp.o
bin/procotol: CMakeFiles/procotol.dir/protocol/protocol.c.o
bin/procotol: CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o
bin/procotol: CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o
bin/procotol: CMakeFiles/procotol.dir/protocol/protocol_server_proc.cpp.o
bin/procotol: CMakeFiles/procotol.dir/utils/mini_utils.c.o
bin/procotol: CMakeFiles/procotol.dir/build.make
bin/procotol: CMakeFiles/procotol.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/workspace/NanTongProtocol2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable bin/procotol"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/procotol.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/procotol.dir/build: bin/procotol

.PHONY : CMakeFiles/procotol.dir/build

CMakeFiles/procotol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/procotol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/procotol.dir/clean

CMakeFiles/procotol.dir/depend:
	cd /workspace/NanTongProtocol2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspace/NanTongProtocol2 /workspace/NanTongProtocol2 /workspace/NanTongProtocol2 /workspace/NanTongProtocol2 /workspace/NanTongProtocol2/CMakeFiles/procotol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/procotol.dir/depend

