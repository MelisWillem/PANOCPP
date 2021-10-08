### make sure the user is doing an our of source build:
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds are not allowed. use cmake -H. -Bbuild instead to build in the build folder" )
endif()
###

cmake_minimum_required (VERSION 3.2)
project (PANOCPP)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("${CMAKE_SOURCE_DIR}/lib/catch2")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/test")

# Test the functionality
enable_testing()

add_executable(panocTests test/PanocTests.cpp)
add_executable(vectorAlgebraTests test/VectorAlgebraTests.cpp)
add_executable(proxOperatorTests test/proxOperatorTest.cpp)

set_property(TARGET panocTests PROPERTY CXX_STANDARD 17)
set_property(TARGET vectorAlgebraTests PROPERTY CXX_STANDARD 17) 
set_property(TARGET proxOperatorTests PROPERTY CXX_STANDARD 17)

if(NOT MSVC)
    target_link_libraries(panocTests m) # link with the math lib, some toolchains need this
endif()

add_test(NAME panocTests COMMAND panocTests)
add_test(NAME vectorAlgebraTests COMMAND vectorAlgebraTests)
add_test(NAME proxOperatorTests COMMAND proxOperatorTests)
