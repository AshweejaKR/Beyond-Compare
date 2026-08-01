qt_add_executable(BCClone
    WIN32_EXECUTABLE
    main.cpp
)

target_link_libraries(BCClone PRIVATE
    Qt6::Widgets
    bcclone_ui
    bcclone_utils
)

bcclone_set_warnings(BCClone)

install(TARGETS BCClone
    RUNTIME DESTINATION bin
)
