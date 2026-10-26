cmake_minimum_required(VERSION 3.15)
project(caut VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Oi /Ot /Gy /DNDEBUG")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
	CAUT_VERSION="${PROJECT_VERSION}"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/patterns
    ${CMAKE_SOURCE_DIR}/bin/patterns
)
