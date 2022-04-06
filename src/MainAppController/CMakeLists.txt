add_subdirectory("lib")

add_library(
    MainAppController
    STATIC
    MainAppController.cc
    MainAppController.h
    "common/AspectRatio.h"
    "common/glmHeaders.h"
    "common/IndexBuffer.cc"
    "common/IndexBuffer.h"
    "common/Renderer.cc"
    "common/Renderer.h"
    "common/Shader.cc"
    "common/Shader.h"
    "common/Texture.cc"
    "common/Texture.h"
    "common/VertexArray.cc"
    "common/VertexArray.h"
    "common/VertexBuffer.cc"
    "common/VertexBuffer.h"
    "common/VertexBufferElement.h"
    "common/VertexBufferLayout.h"
    "common/WindowStats.h"
    "helpers/openGLErrorHelpers.cc"
    "helpers/openGLErrorHelpers.h"
    "listeners/WindowListener.h"
    "managers/ImGuiManager.cc"
    "managers/ImGuiManager.h"
    "managers/ThreadPoolManager.cc"
    "managers/ThreadPoolManager.h"
    "managers/WindowManager.cc"
    "managers/WindowManager.h"
    "models/TestModel.h"
    "tests/Test.h"
    "tests/TestContext.h"
    "tests/TestMenu.cc"
    "tests/TestMenu.h"
)

target_include_directories(
    MainAppController
    SYSTEM
    PRIVATE
    "lib"
)

target_include_directories(
    MainAppController
    PUBLIC
    "common"
    "helpers"
    "listeners"
    "managers"
    "models"
    "tests"
)

target_link_libraries(
    MainAppController
    # Removed PUBLIC while trying to use `static` below
    # PUBLIC
    ExternalLibraries
)

# Apple doesn't have some static lib, like: ctr0.o
# So we only add static linking on Windows, for now...
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "CBR: Static linking Windows")
    target_link_libraries(MainAppController -static)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

install(TARGETS MainAppController DESTINATION lib)
install(FILES MainAppController.h DESTINATION include)
