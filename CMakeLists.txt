cmake_minimum_required(VERSION 3.0.2)
project(ros_mbed_serial)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES ros_mbed_serial
  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
   scripts/gen_sample_serial_topic.py
   scripts/print_msg_from_mbed.py
   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

add_executable(ros_mbed_serial_node src/ros_mbed_serial.cpp)
add_dependencies(ros_mbed_serial_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(ros_mbed_serial_node ${catkin_LIBRARIES})
