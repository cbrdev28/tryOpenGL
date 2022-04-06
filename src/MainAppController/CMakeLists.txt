add_subdirectory("lib")

add_library(
    MainAppController
    STATIC
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
    "tests/TestMenu.cc"
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
