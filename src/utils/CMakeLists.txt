add_library(bcclone_utils STATIC
    Logger.h
    Logger.cpp
    SettingsManager.h
    SettingsManager.cpp
)

target_include_directories(bcclone_utils PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bcclone_utils PUBLIC
    Qt6::Core
)

bcclone_set_warnings(bcclone_utils)
