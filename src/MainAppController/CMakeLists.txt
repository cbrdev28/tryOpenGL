add_subdirectory("lib")

add_library(
    MainAppController
    MainAppController.cc
    "common/IndexBuffer.cc"
    "common/Renderer.cc"
    "common/Shader.cc"
    "common/Texture.cc"
    "common/VertexArray.cc"
    "common/VertexBuffer.cc"
    "helpers/openGLErrorHelpers.cc"
    "managers/ImGuiManager.cc"
    "managers/ThreadPoolManager.cc"
    "managers/WindowManager.cc"
    "tests/TestInstanceTriangle.cc"
    "tests/TestMenu.cc"
)

target_include_directories(
    MainAppController
    SYSTEM
    PRIVATE
    "lib/include"
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
    PUBLIC
    ExternalLibraries
)