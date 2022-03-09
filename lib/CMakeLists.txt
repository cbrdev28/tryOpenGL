add_library(
    ExternalLibraries
    "glad.c"
)

target_include_directories(
    ExternalLibraries
    PUBLIC
    "include/"
)