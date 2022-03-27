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

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    # GIT_TAG v1.87
    GIT_TAG docking
    UPDATE_DISCONNECTED YES
)
FetchContent_MakeAvailable(imgui)
add_library(
    imgui
    STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)
target_include_directories(
    imgui
    SYSTEM
    PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(
    imgui
    PUBLIC
    glfw
)

FetchContent_Declare(
    GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
)

FetchContent_MakeAvailable(GSL)

add_library(
    ExternalLibraries
    "glad/glad.c"
    "stb_image/stb_image.cc"
)

target_include_directories(
    ExternalLibraries
    PUBLIC
    "glad/include/"
    "stb_image/"
)

target_link_libraries(
    ExternalLibraries
    PUBLIC
    GSL
    fmt::fmt
    glfw
    glm
    imgui
)