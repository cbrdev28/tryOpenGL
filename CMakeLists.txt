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

#########################################################
######## Pre-compiled header: fix for clang-tidy ########
# From: https://gitlab.kitware.com/cmake/cmake/-/issues/22081
set(PREFIX_HEADER "${CMAKE_CURRENT_LIST_DIR}/${srcDir}/cbrpch.h")
# Uncomment the rest to fix clang-tidy (TODO: disable in release mode)
# set(CLANG_PCH_OUTPUT "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}.dir/cmake_pch.hxx.pch")
# get_property(CLANG_PCH_COMPILE_OPTIONS TARGET ${PROJECT_NAME} PROPERTY COMPILE_OPTIONS)
# get_property(CLANG_PCH_INCLUDE_DIRECTORIES TARGET ${PROJECT_NAME} PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
# set(CLANG_PCH_INCLUDE_STRING "")
# foreach(dir ${CLANG_PCH_INCLUDE_DIRECTORIES})
#     # Generate system includes to suppress warnings for third-party headers
#     set(CLANG_PCH_INCLUDE_STRING ${CLANG_PCH_INCLUDE_STRING} -isystem ${dir})
# endforeach()
# add_custom_command(
#     OUTPUT "${CLANG_PCH_OUTPUT}"
#     # Mac: instead of `clang++` use `cbr-llvm-clang++`. It breaks the build but will let ctidy script run properly.
#     COMMAND clang++ ${CLANG_PCH_COMPILE_OPTIONS} -std=c++${CMAKE_CXX_STANDARD} ${CLANG_PCH_INCLUDE_STRING} -x c++-header ${PREFIX_HEADER} -Xclang -emit-pch -o ${CLANG_PCH_OUTPUT}
#     COMMENT "Creating Clang precompiled header for clang-tidy."
# )
# add_custom_target(clang-pch ALL
#     DEPENDS ${CLANG_PCH_OUTPUT}
# )
# # Create "fake" precompiled header for clang-tidy
# add_dependencies(${PROJECT_NAME} clang-pch)
# # Create the real GCC precompiled header used by compilation units
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PREFIX_HEADER})
######## Pre-compiled header: fix for clang-tidy ########
#########################################################

# Copy resources
# Shaders
set(resShaderDir "res/shaders")
file(GLOB shaderFiles "${resShaderDir}/*.shader")

set(binResShaderDir "${CMAKE_BINARY_DIR}/${resShaderDir}")
file(MAKE_DIRECTORY ${binResShaderDir})

foreach(file ${shaderFiles})
    configure_file(${file} ${binResShaderDir} COPYONLY)
endforeach(file ${shaderFiles})

# Textures
set(resTextureDir "res/textures")
file(GLOB textureFiles "${resTextureDir}/*.png")

set(binResTextureDir "${CMAKE_BINARY_DIR}/${resTextureDir}")
file(MAKE_DIRECTORY ${binResTextureDir})

foreach(file ${textureFiles})
    configure_file(${file} ${binResTextureDir} COPYONLY)
endforeach(file ${textureFiles})

# Resource config file is a special case, so we can export version numbers
set(resConfigDir "res/config")
configure_file("${resConfigDir}/tryOpenGLConfig.h.in" tryOpenGLConfig.h)

# Install steps
install(TARGETS tryOpenGL DESTINATION bin)
# Install resources (maybe revisit to exlude the `config` folder)
set(resDir "res")
install(DIRECTORY ${resDir} DESTINATION bin)

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
