### make sure the user is doing an our of source build:
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds are not allowed. use cmake -H. -Bbuild instead to build in the build folder" )
endif()
###

cmake_minimum_required (VERSION 2.6)
project (PANOCPP)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    MESSAGE("using clang compiler -> setting c++ version to 17")
    SET (CMAKE_CXX_FLAGS "-Wall -std=c++17")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

include_directories("${CMAKE_SOURCE_DIR}/lib/catch2")
include_directories("${CMAKE_SOURCE_DIR}/include")

# Test the functionality
enable_testing()

add_executable(panocTests test/PanocTests.cpp)
add_executable(vectorAlgTests test/VectorAlgebraTests.cpp)
add_executable(proxOperatorTests test/proxOperatorTest.cpp)

if(NOT MSVC)
    target_link_libraries(panocTests m) # link with the math lib, some toolchains need this
endif()

add_test(NAME panocTests COMMAND panocTests)
add_test(NAME vectorAlgTests COMMAND vectorAlgebraTests)

