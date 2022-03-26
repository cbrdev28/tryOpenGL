add_subdirectory("lib")

add_library(
    MainAppController
    MainAppController.cc
    "managers/WindowManager.cc"
    "managers/ImGuiManager.cc"
    "managers/ShaderManager.cc"
    "helpers/openGLErrorHelpers.cc"
    "common/Renderer.cc"
    "common/VertexBuffer.cc"
    "common/IndexBuffer.cc"
    "common/VertexArray.cc"
    "common/Texture.cc"
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