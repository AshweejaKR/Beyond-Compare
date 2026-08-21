add_library(bcclone_folder STATIC
    FolderComparator.h
    FolderComparator.cpp
    ${CMAKE_SOURCE_DIR}/src/models/FolderDiffModel.h
)

target_include_directories(bcclone_folder PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bcclone_folder PUBLIC
    Qt6::Core
)

bcclone_set_warnings(bcclone_folder)
