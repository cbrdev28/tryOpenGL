cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(tryOpenGL main.cc)

install(TARGETS tryOpenGL DESTINATION bin)

enable_testing()
# Test 1: does the program run
add_test(NAME Runs COMMAND tryOpenGL)

# Test 2: does the program print "Hello world" 
add_test(NAME PrintsHelloWorld COMMAND tryOpenGL)
set_tests_properties(
    PrintsHelloWorld
    PROPERTIES PASS_REGULAR_EXPRESSION "Hello world"
)

# Test 3: does the program print "Hello tests" with "--test" 
add_test(NAME PrintsHelloTests COMMAND tryOpenGL "--test")
set_tests_properties(
    PrintsHelloTests
    PROPERTIES PASS_REGULAR_EXPRESSION "Hello tests"
)
