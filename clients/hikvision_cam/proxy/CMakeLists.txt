cmake_minimum_required(VERSION 3.5)

project(hikvision_alarm_proxy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)
list(APPEND PRJ_SRCS "../../../src/MessageQueue.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Boost::system Boost::thread rt)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${Boost_INCLUDE_DIRS}
    )
