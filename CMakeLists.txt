cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Needed for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("src/MainAppController")
add_subdirectory("lib")

add_executable(tryOpenGL main.cc)

target_compile_options(tryOpenGL PRIVATE -Wall)
if(NOT MSVC)
    target_compile_options(tryOpenGL PRIVATE -Wextra)
endif()

target_include_directories(
    tryOpenGL
    PUBLIC
    "src/MainAppController"
)

target_link_libraries(
    tryOpenGL
    PUBLIC
    MainAppController
    ExternalLibraries
)

# NOTE: I don't know if I should update the install steps
# with the MainAppController library
install(TARGETS tryOpenGL DESTINATION bin)

enable_testing()

# Enable/Disable this test to run the program
add_test(NAME Runs COMMAND tryOpenGL)

# Example of failure when running with --test:
# add_test(NAME RunsTestOpenGL COMMAND tryOpenGL --test opengl)
