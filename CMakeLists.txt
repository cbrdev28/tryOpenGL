cmake_minimum_required(VERSION 3.22)

project(tryOpenGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed for clang-tidy

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DCBR_APPLE)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DCBR_WINDOWS)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

add_subdirectory("lib")

# Set our directories containing source & header files
set(srcDir "src")
set(mainAppDir "src/MainAppController")
set(commonDir "src/MainAppController/common")
set(helpersDir "src/MainAppController/helpers")
set(managersDir "src/MainAppController/managers")
set(testsDir "src/MainAppController/tests")

file (GLOB srcFiles "${srcDir}/*.cc")
file (GLOB mainAppSrcFiles "${mainAppDir}/*.cc")
file (GLOB commonSrcFiles "${commonDir}/*.cc")
file (GLOB helpersSrcFiles "${helpersDir}/*.cc")
file (GLOB managersSrcFiles "${managersDir}/*.cc")
file (GLOB testsSrcFiles "${testsDir}/*.cc")

add_executable(
    tryOpenGL
    ${srcFiles}
    ${mainAppSrcFiles}
    ${commonSrcFiles}
    ${helpersSrcFiles}
    ${managersSrcFiles}
    ${testsSrcFiles}
)

target_compile_options(tryOpenGL PRIVATE -Wall)
if(NOT MSVC)
    target_compile_options(tryOpenGL PRIVATE -Wextra)
endif()

target_include_directories(
    tryOpenGL
    PUBLIC
    ${srcDir}
    ${mainAppDir}
    ${commonDir}
    ${helpersDir}
    ${managersDir}
    ${testsDir}
)

target_link_libraries(
    tryOpenGL
    # PRIVATE (remove because of : `target_link_libraries(tryOpenGL -static)`)
    VendorLibs # from `add_subdirectory("lib")`
)

target_precompile_headers(
    tryOpenGL
    PUBLIC
    "${srcDir}/cbrpch.h"
)

# Apple doesn't have some static lib, like: ctr0.o
# So we only add static linking on Windows, for now...
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(tryOpenGL -static)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Copy resources - to be tested with CPack
# configure_file("res/shaders/test_rectangles.shader" ${CMAKE_BINARY_DIR} COPYONLY)
# configure_file("res/textures/grass.png" ${CMAKE_BINARY_DIR} COPYONLY)
# configure_file("res/textures/wall.png" ${CMAKE_BINARY_DIR} COPYONLY)

# Install steps
install(TARGETS tryOpenGL DESTINATION bin)
# TODO: install more headers? check after making another package on windows

# Install resources - to be tested with CPack
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
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)
