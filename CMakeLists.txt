cmake_minimum_required(VERSION 3.25)
project("2BodyProblem")

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART REQUIRED True)

find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Audio Window System)

add_executable(2BodyProblem main.cpp)

target_link_libraries(2BodyProblem PRIVATE SFML::Graphics SFML::Audio SFML::Window SFML::System)