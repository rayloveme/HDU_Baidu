# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /root/workspace/car

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/car/build

# Include any dependencies generated for this target.
include CMakeFiles/run_car.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/run_car.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/run_car.dir/flags.make

CMakeFiles/run_car.dir/Image_process.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/Image_process.cpp.o: ../Image_process.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/run_car.dir/Image_process.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/Image_process.cpp.o -c /root/workspace/car/Image_process.cpp

CMakeFiles/run_car.dir/Image_process.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/Image_process.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/Image_process.cpp > CMakeFiles/run_car.dir/Image_process.cpp.i

CMakeFiles/run_car.dir/Image_process.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/Image_process.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/Image_process.cpp -o CMakeFiles/run_car.dir/Image_process.cpp.s

CMakeFiles/run_car.dir/Image_process.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/Image_process.cpp.o.requires

CMakeFiles/run_car.dir/Image_process.cpp.o.provides: CMakeFiles/run_car.dir/Image_process.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/Image_process.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/Image_process.cpp.o.provides

CMakeFiles/run_car.dir/Image_process.cpp.o.provides.build: CMakeFiles/run_car.dir/Image_process.cpp.o


CMakeFiles/run_car.dir/main.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/run_car.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/main.cpp.o -c /root/workspace/car/main.cpp

CMakeFiles/run_car.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/main.cpp > CMakeFiles/run_car.dir/main.cpp.i

CMakeFiles/run_car.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/main.cpp -o CMakeFiles/run_car.dir/main.cpp.s

CMakeFiles/run_car.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/main.cpp.o.requires

CMakeFiles/run_car.dir/main.cpp.o.provides: CMakeFiles/run_car.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/main.cpp.o.provides

CMakeFiles/run_car.dir/main.cpp.o.provides.build: CMakeFiles/run_car.dir/main.cpp.o


CMakeFiles/run_car.dir/ring.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/ring.cpp.o: ../ring.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/run_car.dir/ring.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/ring.cpp.o -c /root/workspace/car/ring.cpp

CMakeFiles/run_car.dir/ring.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/ring.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/ring.cpp > CMakeFiles/run_car.dir/ring.cpp.i

CMakeFiles/run_car.dir/ring.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/ring.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/ring.cpp -o CMakeFiles/run_car.dir/ring.cpp.s

CMakeFiles/run_car.dir/ring.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/ring.cpp.o.requires

CMakeFiles/run_car.dir/ring.cpp.o.provides: CMakeFiles/run_car.dir/ring.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/ring.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/ring.cpp.o.provides

CMakeFiles/run_car.dir/ring.cpp.o.provides.build: CMakeFiles/run_car.dir/ring.cpp.o


CMakeFiles/run_car.dir/sancha.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/sancha.cpp.o: ../sancha.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/run_car.dir/sancha.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/sancha.cpp.o -c /root/workspace/car/sancha.cpp

CMakeFiles/run_car.dir/sancha.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/sancha.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/sancha.cpp > CMakeFiles/run_car.dir/sancha.cpp.i

CMakeFiles/run_car.dir/sancha.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/sancha.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/sancha.cpp -o CMakeFiles/run_car.dir/sancha.cpp.s

CMakeFiles/run_car.dir/sancha.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/sancha.cpp.o.requires

CMakeFiles/run_car.dir/sancha.cpp.o.provides: CMakeFiles/run_car.dir/sancha.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/sancha.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/sancha.cpp.o.provides

CMakeFiles/run_car.dir/sancha.cpp.o.provides.build: CMakeFiles/run_car.dir/sancha.cpp.o


CMakeFiles/run_car.dir/self_serial.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/self_serial.cpp.o: ../self_serial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/run_car.dir/self_serial.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/self_serial.cpp.o -c /root/workspace/car/self_serial.cpp

CMakeFiles/run_car.dir/self_serial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/self_serial.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/self_serial.cpp > CMakeFiles/run_car.dir/self_serial.cpp.i

CMakeFiles/run_car.dir/self_serial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/self_serial.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/self_serial.cpp -o CMakeFiles/run_car.dir/self_serial.cpp.s

CMakeFiles/run_car.dir/self_serial.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/self_serial.cpp.o.requires

CMakeFiles/run_car.dir/self_serial.cpp.o.provides: CMakeFiles/run_car.dir/self_serial.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/self_serial.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/self_serial.cpp.o.provides

CMakeFiles/run_car.dir/self_serial.cpp.o.provides.build: CMakeFiles/run_car.dir/self_serial.cpp.o


CMakeFiles/run_car.dir/zsh_construction.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/zsh_construction.cpp.o: ../zsh_construction.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/run_car.dir/zsh_construction.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/zsh_construction.cpp.o -c /root/workspace/car/zsh_construction.cpp

CMakeFiles/run_car.dir/zsh_construction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/zsh_construction.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/zsh_construction.cpp > CMakeFiles/run_car.dir/zsh_construction.cpp.i

CMakeFiles/run_car.dir/zsh_construction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/zsh_construction.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/zsh_construction.cpp -o CMakeFiles/run_car.dir/zsh_construction.cpp.s

CMakeFiles/run_car.dir/zsh_construction.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/zsh_construction.cpp.o.requires

CMakeFiles/run_car.dir/zsh_construction.cpp.o.provides: CMakeFiles/run_car.dir/zsh_construction.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/zsh_construction.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/zsh_construction.cpp.o.provides

CMakeFiles/run_car.dir/zsh_construction.cpp.o.provides.build: CMakeFiles/run_car.dir/zsh_construction.cpp.o


CMakeFiles/run_car.dir/zsh_image_deal.cpp.o: CMakeFiles/run_car.dir/flags.make
CMakeFiles/run_car.dir/zsh_image_deal.cpp.o: ../zsh_image_deal.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/run_car.dir/zsh_image_deal.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run_car.dir/zsh_image_deal.cpp.o -c /root/workspace/car/zsh_image_deal.cpp

CMakeFiles/run_car.dir/zsh_image_deal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run_car.dir/zsh_image_deal.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/car/zsh_image_deal.cpp > CMakeFiles/run_car.dir/zsh_image_deal.cpp.i

CMakeFiles/run_car.dir/zsh_image_deal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run_car.dir/zsh_image_deal.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/car/zsh_image_deal.cpp -o CMakeFiles/run_car.dir/zsh_image_deal.cpp.s

CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.requires:

.PHONY : CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.requires

CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.provides: CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.requires
	$(MAKE) -f CMakeFiles/run_car.dir/build.make CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.provides.build
.PHONY : CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.provides

CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.provides.build: CMakeFiles/run_car.dir/zsh_image_deal.cpp.o


# Object files for target run_car
run_car_OBJECTS = \
"CMakeFiles/run_car.dir/Image_process.cpp.o" \
"CMakeFiles/run_car.dir/main.cpp.o" \
"CMakeFiles/run_car.dir/ring.cpp.o" \
"CMakeFiles/run_car.dir/sancha.cpp.o" \
"CMakeFiles/run_car.dir/self_serial.cpp.o" \
"CMakeFiles/run_car.dir/zsh_construction.cpp.o" \
"CMakeFiles/run_car.dir/zsh_image_deal.cpp.o"

# External object files for target run_car
run_car_EXTERNAL_OBJECTS =

run_car: CMakeFiles/run_car.dir/Image_process.cpp.o
run_car: CMakeFiles/run_car.dir/main.cpp.o
run_car: CMakeFiles/run_car.dir/ring.cpp.o
run_car: CMakeFiles/run_car.dir/sancha.cpp.o
run_car: CMakeFiles/run_car.dir/self_serial.cpp.o
run_car: CMakeFiles/run_car.dir/zsh_construction.cpp.o
run_car: CMakeFiles/run_car.dir/zsh_image_deal.cpp.o
run_car: CMakeFiles/run_car.dir/build.make
run_car: /usr/local/lib/libopencv_dnn.so.3.4.3
run_car: /usr/local/lib/libopencv_ml.so.3.4.3
run_car: /usr/local/lib/libopencv_objdetect.so.3.4.3
run_car: /usr/local/lib/libopencv_shape.so.3.4.3
run_car: /usr/local/lib/libopencv_stitching.so.3.4.3
run_car: /usr/local/lib/libopencv_superres.so.3.4.3
run_car: /usr/local/lib/libopencv_videostab.so.3.4.3
run_car: /usr/local/lib/libopencv_calib3d.so.3.4.3
run_car: /usr/local/lib/libopencv_features2d.so.3.4.3
run_car: /usr/local/lib/libopencv_flann.so.3.4.3
run_car: /usr/local/lib/libopencv_highgui.so.3.4.3
run_car: /usr/local/lib/libopencv_photo.so.3.4.3
run_car: /usr/local/lib/libopencv_video.so.3.4.3
run_car: /usr/local/lib/libopencv_videoio.so.3.4.3
run_car: /usr/local/lib/libopencv_imgcodecs.so.3.4.3
run_car: /usr/local/lib/libopencv_imgproc.so.3.4.3
run_car: /usr/local/lib/libopencv_core.so.3.4.3
run_car: CMakeFiles/run_car.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/car/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable run_car"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/run_car.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/run_car.dir/build: run_car

.PHONY : CMakeFiles/run_car.dir/build

CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/Image_process.cpp.o.requires
CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/main.cpp.o.requires
CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/ring.cpp.o.requires
CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/sancha.cpp.o.requires
CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/self_serial.cpp.o.requires
CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/zsh_construction.cpp.o.requires
CMakeFiles/run_car.dir/requires: CMakeFiles/run_car.dir/zsh_image_deal.cpp.o.requires

.PHONY : CMakeFiles/run_car.dir/requires

CMakeFiles/run_car.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/run_car.dir/cmake_clean.cmake
.PHONY : CMakeFiles/run_car.dir/clean

CMakeFiles/run_car.dir/depend:
	cd /root/workspace/car/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/car /root/workspace/car /root/workspace/car/build /root/workspace/car/build /root/workspace/car/build/CMakeFiles/run_car.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/run_car.dir/depend
