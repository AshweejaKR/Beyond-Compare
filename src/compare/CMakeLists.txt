add_library(bcclone_compare STATIC
    TextCompareEngine.h
    TextCompareEngine.cpp
    TextFileLoader.h
    TextFileLoader.cpp
    TextNormalizer.h
    TextNormalizer.cpp
    ${CMAKE_SOURCE_DIR}/src/models/DiffModel.h
)

target_include_directories(bcclone_compare PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bcclone_compare PUBLIC
    Qt6::Core
)

bcclone_set_warnings(bcclone_compare)
