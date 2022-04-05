cmake_minimum_required(VERSION 3.22)

project(tryOpenGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed for clang-tidy

# Check platform
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message(STATUS "CBR: Platform is Mac OS X")
    add_definitions(-DCBR_APPLE)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "CBR: Platform is Windows")
    add_definitions(-DCBR_WINDOWS)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

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

# Copy resources
# configure_file("res/shaders/test_rectangles.shader" ${CMAKE_BINARY_DIR} COPYONLY)

# configure_file("res/textures/grass.png" ${CMAKE_BINARY_DIR} COPYONLY)
# configure_file("res/textures/wall.png" ${CMAKE_BINARY_DIR} COPYONLY)

# Install steps
install(TARGETS tryOpenGL DESTINATION bin)

# Install resources
# install(FILES "res/shaders/test_rectangles.shader" DESTINATION bin)

# install(FILES "res/textures/grass.png" DESTINATION bin)
# install(FILES "res/textures/wall.png" DESTINATION bin)

# Testing steps
enable_testing()

# Enable/Disable this test to run the program
add_test(NAME Runs COMMAND tryOpenGL)

# Enable packaging with CPack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${tryOpenGL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${tryOpenGL_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${tryOpenGL_VERSION_PATCH}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)