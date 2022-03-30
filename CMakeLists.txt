cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed for clang-tidy

add_subdirectory("src/MainAppController")

add_executable(tryOpenGL "src/main.cc")

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

configure_file("res/shaders/test_instance_triangle.shader" ${CMAKE_BINARY_DIR} COPYONLY)

configure_file("res/textures/grass.png" ${CMAKE_BINARY_DIR} COPYONLY)
configure_file("res/textures/wall.png" ${CMAKE_BINARY_DIR} COPYONLY)

# Install steps
install(TARGETS tryOpenGL DESTINATION bin)

install(FILES "res/shaders/test_instance_triangle.shader" DESTINATION bin)

install(FILES "res/textures/grass.png" DESTINATION bin)
install(FILES "res/textures/wall.png" DESTINATION bin)

# Testing steps
enable_testing()

# Enable/Disable this test to run the program
add_test(NAME Runs COMMAND tryOpenGL)
