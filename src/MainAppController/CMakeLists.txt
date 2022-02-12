add_library(MainAppController MainAppController.cc)

target_link_libraries(
    MainAppController 
    PRIVATE
    fmt::fmt
)