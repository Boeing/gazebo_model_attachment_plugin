cmake_minimum_required(VERSION 3.0.2)
project(boeing_gazebo_model_attachment_plugin)

add_definitions(-std=c++14)

find_package(catkin REQUIRED COMPONENTS gazebo_msgs gazebo_ros message_generation roscpp)

find_package(gazebo REQUIRED)

catkin_python_setup()

add_service_files(FILES Attach.srv Detach.srv)

generate_messages()

catkin_package(
    INCLUDE_DIRS
    include
    LIBRARIES
    ${PROJECT_NAME}
    CATKIN_DEPENDS
    gazebo_msgs
    gazebo_ros
    message_generation
    message_runtime
    roscpp
)

add_library(${PROJECT_NAME} include/${PROJECT_NAME}/${PROJECT_NAME}.h src/${PROJECT_NAME}.cpp)

target_link_libraries(${PROJECT_NAME} ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} ${catkin_LIBRARIES})

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

include_directories(include ${catkin_INCLUDE_DIRS})

include_directories(SYSTEM ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

install(
    DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    FILES_MATCHING
    PATTERN "*.h"
)

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(DIRECTORY test DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

if(CATKIN_ENABLE_TESTING)
    find_package(rostest REQUIRED)
    add_rostest(test/test_plugin.test)
endif()
