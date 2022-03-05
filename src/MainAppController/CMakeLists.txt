add_library(
    MainAppController
    MainAppController.cc
    "managers/WindowManager.cc"
    "managers/InputManager.cc"
    "managers/WorldManager.cc"
    "managers/ShaderManager.cc"
    "helpers/MatrixHelper.cc"
)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(glm)

set(FMT_SYSTEM_HEADERS ON CACHE INTERNAL "Expose headers with marking them as system.")
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.1.1
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.6
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(glfw)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "NOT Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "NOT Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "NOT Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "NOT Generate installation target")

target_link_libraries(
    MainAppController 
    PUBLIC
    fmt::fmt
    glfw
    glm
)

target_include_directories(
    MainAppController
    SYSTEM
    PUBLIC
    "../../lib/include"
    "common"
    "listeners"
    "helpers"
)