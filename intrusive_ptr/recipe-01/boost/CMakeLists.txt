cmake_minimum_required( VERSION 3.15 )

project ( intrusive_ptr_demo LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS smart_ptr)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_executable( intrusive_ptr_demo ${SOURCES} )
target_link_libraries( intrusive_ptr_demo Boost::smart_ptr )

