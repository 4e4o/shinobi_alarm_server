cmake_minimum_required(VERSION 3.5)

add_subdirectory(clients/hikvision_cam/client)
add_subdirectory(clients/hikvision_cam/proxy)
add_subdirectory(clients/notSofia)

project(shinobi_alarm_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread program_options)

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Boost::system Boost::thread Boost::program_options rt)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${Boost_INCLUDE_DIRS}
    )
