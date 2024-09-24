cmake_minimum_required(VERSION 3.8)

project(gnss_listener VERSION 0.0.1)

set(std_msgs_INCLUDE_DIRS /opt/ros/humble/include)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(gnss_someip_lib REQUIRED)
find_package(std_msgs REQUIRED)

set(GNSS_LISTENER_TARGET gnss-listener)

add_executable(${GNSS_LISTENER_TARGET} src/main.cpp)

ament_target_dependencies(${GNSS_LISTENER_TARGET} rclcpp gnss_someip_lib std_msgs)

# include_directories(
#   ${std_msgs_INCLUDE_DIRS}
# )

# target_link_libraries(${PROJECT_NAME} PRIVATE
#     rosidl_typesupport_cpp
# )

target_compile_features(${GNSS_LISTENER_TARGET} PRIVATE cxx_std_17)

install(TARGETS ${GNSS_LISTENER_TARGET} DESTINATION lib/${PROJECT_NAME})

ament_package()
