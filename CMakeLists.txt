cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed for clang-tidy

add_subdirectory("src/MainAppController")

add_executable(tryOpenGL main.cc)

target_compile_options(tryOpenGL PRIVATE -Wall)
if(NOT MSVC)
    target_compile_options(tryOpenGL PRIVATE -Wextra)
endif()

target_include_directories(
    tryOpenGL
    PRIVATE
    "src/MainAppController"
)

target_link_libraries(
    tryOpenGL
    PRIVATE
    MainAppController
)

configure_file("res/shaders/basic.shader" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("res/shaders/test_batch_render.shader" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("res/shaders/test_render_tiles.shader" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("res/shaders/test_texture.shader" ${CMAKE_BINARY_DIR} COPYONLY)

install(TARGETS tryOpenGL DESTINATION bin)

enable_testing()

# Enable/Disable this test to run the program
add_test(NAME Runs COMMAND tryOpenGL)

# Example of failure when running with --test:
# add_test(NAME RunsTestOpenGL COMMAND tryOpenGL --test opengl)
