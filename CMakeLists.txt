cmake_minimum_required(VERSION 3.5)

project(shinobi_alarm_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(boost_base)

find_package(Boost REQUIRED COMPONENTS filesystem)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_link_libraries(${PROJECT_NAME} boost_base Boost::filesystem)

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

install(FILES "scripts/shinobi_alarm_server.service" DESTINATION /etc/systemd/system/)
install(FILES "scripts/config.txt" DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/shinobi_alarm_server/)
