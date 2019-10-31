### make sure the user is doing an our of source build:
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds are not allowed. use cmake -H. -Bbuild instead to build in the build folder" )
endif()
###

cmake_minimum_required (VERSION 2.6)
project (PANOCPP)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(lib/catch2)

# Test the functionality
enable_testing()

add_executable(dummy test/dummy.cpp)

