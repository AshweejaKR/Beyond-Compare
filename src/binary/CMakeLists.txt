add_library(bcclone_binary STATIC
    BinaryCompareEngine.h
    BinaryCompareEngine.cpp
    ${CMAKE_SOURCE_DIR}/src/models/BinaryDiffModel.h
)

target_include_directories(bcclone_binary PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bcclone_binary PUBLIC
    Qt6::Core
)

bcclone_set_warnings(bcclone_binary)
