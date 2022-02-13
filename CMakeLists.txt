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
