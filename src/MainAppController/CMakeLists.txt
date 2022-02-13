add_library(
    MainAppController
    MainAppController.cc
    "../glad.c"
    "managers/WindowManager.cc"
    "managers/InputManager.cc"
    "managers/WorldManager.cc"
    "managers/ShaderManager.cc"
)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(glm)

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

target_include_directories(
    MainAppController
    PUBLIC
    "../include"
)

target_link_libraries(
    MainAppController 
    PUBLIC
    fmt::fmt
    glfw
    glm
)
