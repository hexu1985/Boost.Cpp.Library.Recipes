cmake_minimum_required( VERSION 3.15 )

project(timer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

add_subdirectory(lib)

add_executable(timer_wait timer_wait.cpp)
target_link_libraries(timer_wait timer)

add_executable(timer_once1 timer_once1.cpp)
target_link_libraries(timer_once1 timer)
