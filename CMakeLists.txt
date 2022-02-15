cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory("src/MainAppController")

add_executable(tryOpenGL main.cc)

target_include_directories(
    tryOpenGL
    PUBLIC
    "src/MainAppController"
)

target_link_libraries(
    tryOpenGL
    PUBLIC
    MainAppController
)

# NOTE: I don't know if I should update the install steps
# with the MainAppController library
install(TARGETS tryOpenGL DESTINATION bin)

enable_testing()

# Enable/Disable this test to run the program
add_test(NAME Runs COMMAND tryOpenGL)

# Example of failure when running with --test:
# add_test(NAME RunsTestOpenGL COMMAND tryOpenGL --test opengl)
