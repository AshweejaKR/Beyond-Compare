add_library(bcclone_ui STATIC
    MainWindow.h
    MainWindow.cpp
    TextCompareWidget.h
    TextCompareWidget.cpp
)

target_include_directories(bcclone_ui PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bcclone_ui PUBLIC
    Qt6::Widgets
    bcclone_compare
    bcclone_utils
)

bcclone_set_warnings(bcclone_ui)
