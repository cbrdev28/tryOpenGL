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

# Resources
file(GLOB shaderFiles "res/shaders/*.shader")
file(GLOB textureFiles "res/textures/*.png")

add_executable(
    tryOpenGL
    ${srcFiles}
    ${mainAppSrcFiles}
    ${commonSrcFiles}
    ${helpersSrcFiles}
    ${managersSrcFiles}
    ${testsSrcFiles}
    # Seen from examples: we could also add the resources here (shaders, textures, ...)?
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
    # For the config file `tryOpenGLConfig.h`
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    tryOpenGL
    # PRIVATE (remove because of : `target_link_libraries(tryOpenGL -static)`)
    VendorLibs # from `add_subdirectory("lib")`
)

# Apple doesn't have some static lib, like: ctr0.o
# So we only add static linking on Windows, for now...
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(tryOpenGL -static)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# Using precompiled headers breaks my script for clang-ctidy
# This worked on Windows but only in debug build:
# https://gitlab.kitware.com/cmake/cmake/-/issues/22081
set(PREFIX_HEADER "${CMAKE_CURRENT_LIST_DIR}/${srcDir}/cbrpch.h")
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PREFIX_HEADER})

# Copy resources
# Shaders
foreach(file ${shaderFiles})
    configure_file(${file} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach(file ${shaderFiles})

# Textures
foreach(file ${textureFiles})
    configure_file(${file} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach(file ${textureFiles})

# Resource config file is a special case, so we can export version numbers
set(resConfigDir "res/config")
configure_file("${resConfigDir}/tryOpenGLConfig.h.in" tryOpenGLConfig.h)

# Install steps
install(TARGETS tryOpenGL DESTINATION bin)
# Install resources
install(FILES ${shaderFiles} DESTINATION bin)
install(FILES ${textureFiles} DESTINATION bin)

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
