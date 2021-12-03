cmake_minimum_required(VERSION 3.5)

project(hikvision_alarm_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(HikProtocolLib)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE PRJ_SRCS src/*.cpp src/*.h src/*.hpp)
list(APPEND PRJ_SRCS "../../../src/MessageQueue.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})

target_link_libraries(${PROJECT_NAME} PUBLIC HikProtocolLib)

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
