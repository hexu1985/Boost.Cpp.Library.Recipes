cmake_minimum_required( VERSION 3.15 )

project ( signal_set LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

set(ASIO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/asio/include)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FOREACH (src_file ${SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file})
    target_compile_options(${exe_file} PUBLIC -DASIO_STANDALONE)
    target_include_directories(${exe_file} PUBLIC ${ASIO_INCLUDE})
    #    target_link_libraries(${exe_file} Boost::system)
ENDFOREACH()
