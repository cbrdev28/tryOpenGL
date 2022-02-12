add_library(MainAppController MainAppController.cc "managers/WindowManager.cc")

target_link_libraries(
    MainAppController 
    PRIVATE
    fmt::fmt
)