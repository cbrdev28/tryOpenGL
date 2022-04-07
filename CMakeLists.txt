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

file (GLOB srcFiles "${srcDir}/*.cc" "${srcDir}/*.h")
file (GLOB mainAppSrcFiles "${mainAppDir}/*.cc" "${mainAppDir}/*.h")
file (GLOB commonSrcFiles "${commonDir}/*.cc" "${commonDir}/*.h")
file (GLOB helpersSrcFiles "${helpersDir}/*.cc" "${helpersDir}/*.h")
file (GLOB managersSrcFiles "${managersDir}/*.cc" "${managersDir}/*.h")
file (GLOB testsSrcFiles "${testsDir}/*.cc" "${testsDir}/*.h")

add_executable(
    tryOpenGL
    ${srcFiles}
    ${mainAppSrcFiles}
    ${commonSrcFiles}
    ${helpersSrcFiles}
    ${managersSrcFiles}
    ${testsSrcFiles}
    # Seen from examples: we could also add the resources here (shaders, textures, ...)
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


######## Pre-compiled header: fix for clang-tidy ########
# From: https://gitlab.kitware.com/cmake/cmake/-/issues/22081
set(PREFIX_HEADER "${CMAKE_CURRENT_LIST_DIR}/${srcDir}/cbrpch.h")
set(CLANG_PCH_OUTPUT "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}.dir/cmake_pch.hxx.pch")
get_property(CLANG_PCH_COMPILE_OPTIONS TARGET ${PROJECT_NAME} PROPERTY COMPILE_OPTIONS)
get_property(CLANG_PCH_INCLUDE_DIRECTORIES TARGET ${PROJECT_NAME} PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
set(CLANG_PCH_INCLUDE_STRING "")
foreach(dir ${CLANG_PCH_INCLUDE_DIRECTORIES})
    # Generate system includes to suppress warnings for third-party headers
    set(CLANG_PCH_INCLUDE_STRING ${CLANG_PCH_INCLUDE_STRING} -isystem ${dir})
endforeach()
add_custom_command(
    OUTPUT "${CLANG_PCH_OUTPUT}"
    # Check if we could use `CMAKE_CXX_STANDARD` instead of `-std=c++17`
    COMMAND clang++ ${CLANG_PCH_COMPILE_OPTIONS} -std=c++17 ${CLANG_PCH_INCLUDE_STRING} -x c++-header ${PREFIX_HEADER} -Xclang -emit-pch -o ${CLANG_PCH_OUTPUT}
    COMMENT "Creating Clang precompiled header for clang-tidy."
)
add_custom_target(clang-pch ALL
    DEPENDS ${CLANG_PCH_OUTPUT}
)
# Create "fake" precompiled header for clang-tidy
add_dependencies(${PROJECT_NAME} clang-pch)
# Create the real GCC precompiled header used by compilation units
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PREFIX_HEADER})
######## Pre-compiled header: fix for clang-tidy ########


# Apple doesn't have some static lib, like: ctr0.o
# So we only add static linking on Windows, for now...
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(tryOpenGL -static)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Copy resources - to be tested with CPack
# Try to use GLOB
# configure_file("res/shaders/test_rectangles.shader" ${CMAKE_BINARY_DIR} COPYONLY)
# configure_file("res/textures/grass.png" ${CMAKE_BINARY_DIR} COPYONLY)
# configure_file("res/textures/wall.png" ${CMAKE_BINARY_DIR} COPYONLY)

# Install steps
install(TARGETS tryOpenGL DESTINATION bin)
# TODO: install more headers? check after making another package on windows

# Install resources - to be tested with CPack
# Try to use GLOB
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
# Investigate this:
# set(CPACK_MONOLITHIC_INSTALL 1)
include(CPack)
