cmake_minimum_required(VERSION 3.15)
project(boeing_gazebo_model_attachment_plugin_msgs)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(ament_cmake_cpplint REQUIRED)
find_package(std_msgs REQUIRED)

# Locate msg and srv files
set(srv_files "srv/Attach.srv" "srv/Detach.srv")

# Set target names
set(idl_target ${PROJECT_NAME})
set(lib_target ${PROJECT_NAME}_lib)

rosidl_generate_interfaces(${PROJECT_NAME} ${srv_files} DEPENDENCIES std_msgs)

ament_package()
