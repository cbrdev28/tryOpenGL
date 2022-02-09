cmake_minimum_required(VERSION 3.22)

project(tryOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(tryOpenGL main.cc)

install(TARGETS tryOpenGL DESTINATION bin)

enable_testing()
# Does the application run
add_test(NAME Runs COMMAND tryOpenGL)
