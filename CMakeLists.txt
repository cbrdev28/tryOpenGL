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

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.1.1
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(fmt)

add_subdirectory("src/MainAppController")

add_executable(tryOpenGL main.cc "src/glad.c")

target_include_directories(
    tryOpenGL
    PUBLIC
    "src/include"
    "src/MainAppController"
)

target_link_libraries(
    tryOpenGL 
    PRIVATE
    glfw
    glm
)

target_link_libraries(
    tryOpenGL
    PUBLIC
    MainAppController
)

install(TARGETS tryOpenGL DESTINATION bin)

enable_testing()

# Test 1: does the program run with --test
add_test(NAME RunsTests COMMAND tryOpenGL --test)

# Test 2: does the program print "Hello tests" with --test
add_test(NAME PrintsHelloTests COMMAND tryOpenGL --test)
set_tests_properties(
    PrintsHelloTests
    PROPERTIES PASS_REGULAR_EXPRESSION "Hello tests"
)

# Test 2: does the program run --test first-window
add_test(NAME RunsTestFirstWindow COMMAND tryOpenGL --test first-window)

# Enable this test to run the program
# add_test(NAME Runs COMMAND tryOpenGL)

# Example of failure when running with --test:
# add_test(NAME RunsTestOpenGL COMMAND tryOpenGL --test opengl)
