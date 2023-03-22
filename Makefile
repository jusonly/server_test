# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /workspace/NanTongProtocol2/CMakeFiles /workspace/NanTongProtocol2//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /workspace/NanTongProtocol2/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named procotol

# Build rule for target.
procotol: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 procotol
.PHONY : procotol

# fast build rule for target.
procotol/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/build
.PHONY : procotol/fast

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/main.cpp.s
.PHONY : main.cpp.s

protocol/protocol.o: protocol/protocol.c.o

.PHONY : protocol/protocol.o

# target to build an object file
protocol/protocol.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol.c.o
.PHONY : protocol/protocol.c.o

protocol/protocol.i: protocol/protocol.c.i

.PHONY : protocol/protocol.i

# target to preprocess a source file
protocol/protocol.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol.c.i
.PHONY : protocol/protocol.c.i

protocol/protocol.s: protocol/protocol.c.s

.PHONY : protocol/protocol.s

# target to generate assembly for a file
protocol/protocol.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol.c.s
.PHONY : protocol/protocol.c.s

protocol/protocol_client_proc.o: protocol/protocol_client_proc.c.o

.PHONY : protocol/protocol_client_proc.o

# target to build an object file
protocol/protocol_client_proc.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.o
.PHONY : protocol/protocol_client_proc.c.o

protocol/protocol_client_proc.i: protocol/protocol_client_proc.c.i

.PHONY : protocol/protocol_client_proc.i

# target to preprocess a source file
protocol/protocol_client_proc.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.i
.PHONY : protocol/protocol_client_proc.c.i

protocol/protocol_client_proc.s: protocol/protocol_client_proc.c.s

.PHONY : protocol/protocol_client_proc.s

# target to generate assembly for a file
protocol/protocol_client_proc.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_client_proc.c.s
.PHONY : protocol/protocol_client_proc.c.s

protocol/protocol_prco_abstruct.o: protocol/protocol_prco_abstruct.c.o

.PHONY : protocol/protocol_prco_abstruct.o

# target to build an object file
protocol/protocol_prco_abstruct.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.o
.PHONY : protocol/protocol_prco_abstruct.c.o

protocol/protocol_prco_abstruct.i: protocol/protocol_prco_abstruct.c.i

.PHONY : protocol/protocol_prco_abstruct.i

# target to preprocess a source file
protocol/protocol_prco_abstruct.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.i
.PHONY : protocol/protocol_prco_abstruct.c.i

protocol/protocol_prco_abstruct.s: protocol/protocol_prco_abstruct.c.s

.PHONY : protocol/protocol_prco_abstruct.s

# target to generate assembly for a file
protocol/protocol_prco_abstruct.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_prco_abstruct.c.s
.PHONY : protocol/protocol_prco_abstruct.c.s

protocol/protocol_server_proc.o: protocol/protocol_server_proc.c.o

.PHONY : protocol/protocol_server_proc.o

# target to build an object file
protocol/protocol_server_proc.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_server_proc.c.o
.PHONY : protocol/protocol_server_proc.c.o

protocol/protocol_server_proc.i: protocol/protocol_server_proc.c.i

.PHONY : protocol/protocol_server_proc.i

# target to preprocess a source file
protocol/protocol_server_proc.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_server_proc.c.i
.PHONY : protocol/protocol_server_proc.c.i

protocol/protocol_server_proc.s: protocol/protocol_server_proc.c.s

.PHONY : protocol/protocol_server_proc.s

# target to generate assembly for a file
protocol/protocol_server_proc.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/protocol/protocol_server_proc.c.s
.PHONY : protocol/protocol_server_proc.c.s

utils/mini_utils.o: utils/mini_utils.c.o

.PHONY : utils/mini_utils.o

# target to build an object file
utils/mini_utils.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/utils/mini_utils.c.o
.PHONY : utils/mini_utils.c.o

utils/mini_utils.i: utils/mini_utils.c.i

.PHONY : utils/mini_utils.i

# target to preprocess a source file
utils/mini_utils.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/utils/mini_utils.c.i
.PHONY : utils/mini_utils.c.i

utils/mini_utils.s: utils/mini_utils.c.s

.PHONY : utils/mini_utils.s

# target to generate assembly for a file
utils/mini_utils.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/procotol.dir/build.make CMakeFiles/procotol.dir/utils/mini_utils.c.s
.PHONY : utils/mini_utils.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... procotol"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... protocol/protocol.o"
	@echo "... protocol/protocol.i"
	@echo "... protocol/protocol.s"
	@echo "... protocol/protocol_client_proc.o"
	@echo "... protocol/protocol_client_proc.i"
	@echo "... protocol/protocol_client_proc.s"
	@echo "... protocol/protocol_prco_abstruct.o"
	@echo "... protocol/protocol_prco_abstruct.i"
	@echo "... protocol/protocol_prco_abstruct.s"
	@echo "... protocol/protocol_server_proc.o"
	@echo "... protocol/protocol_server_proc.i"
	@echo "... protocol/protocol_server_proc.s"
	@echo "... utils/mini_utils.o"
	@echo "... utils/mini_utils.i"
	@echo "... utils/mini_utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
