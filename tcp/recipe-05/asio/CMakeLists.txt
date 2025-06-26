cmake_minimum_required( VERSION 3.15 )

project ( tcp_shutdown LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message( STATUS "CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}" )
message( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}" )

set(TEST_SCRIPTS test.sh file_server.cpp)

foreach(script ${TEST_SCRIPTS})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${script}
        #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${script}
                ${CMAKE_CURRENT_BINARY_DIR}/${script}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${script}
        COMMENT "Copying test script ${script}"
    )
endforeach()

add_custom_target(copy_scripts ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/test.sh
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/file_server.cpp
)

set(ASIO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/asio/include)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FOREACH (src_file ${SOURCES})
    get_filename_component(exe_file ${src_file} NAME_WLE)
    add_executable(${exe_file} ${src_file})
    target_compile_options(${exe_file} PUBLIC -DASIO_STANDALONE)
    target_include_directories(${exe_file} PUBLIC ${ASIO_INCLUDE})
    add_dependencies(${exe_file} copy_scripts)
    #    target_link_libraries(${exe_file} Boost::system)
ENDFOREACH()
