cmake_minimum_required( VERSION 3.15 )

project(signal_demo LANGUAGES C)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

FOREACH (src_file ${SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file})
    #    target_link_libraries(${exe_file} tlpi)
ENDFOREACH()

