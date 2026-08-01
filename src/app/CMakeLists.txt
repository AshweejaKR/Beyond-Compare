qt_add_executable(BCClone
    main.cpp
)

set_target_properties(BCClone PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(BCClone PRIVATE
    Qt6::Widgets
    bcclone_ui
    bcclone_utils
)

bcclone_set_warnings(BCClone)

# Qt does not ship a static platform integration: without the "windows"
# plugin next to the executable, QApplication aborts at startup with
# "no Qt platform plugin could be initialized". Deploy it after every
# build so the app runs directly from the build directory.
if(WIN32 AND TARGET Qt6::QWindowsIntegrationPlugin)
    add_custom_command(TARGET BCClone POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:BCClone>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:Qt6::QWindowsIntegrationPlugin>"
            "$<TARGET_FILE_DIR:BCClone>/platforms"
        COMMENT "Deploying Qt platform plugin"
    )
endif()

install(TARGETS BCClone
    RUNTIME DESTINATION bin
)
