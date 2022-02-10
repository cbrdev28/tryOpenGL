cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.6
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(glfw)

add_executable(tryOpenGL main.cc "src/glad.c")

target_include_directories(
    tryOpenGL
    PUBLIC
    "src/include"
)

target_link_libraries(
    tryOpenGL 
    PRIVATE
    glfw
)

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
