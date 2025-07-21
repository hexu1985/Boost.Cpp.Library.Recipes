cmake_minimum_required( VERSION 3.15 )

project(tpli-lib LANGUAGES C)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ename.c.inc
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/Build_ename.sh > ${CMAKE_CURRENT_BINARY_DIR}/ename.c.inc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating ename.c.inc"
)

add_custom_target(
    GenerateEnameFile
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ename.c.inc
    COMMENT "Explicit target to run the custom command"
)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(tlpi 
    OBJECT 
    ${SOURCES}
    ${HEADERS}
    )

target_include_directories(tlpi PUBLIC . PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(tlpi GenerateEnameFile)
