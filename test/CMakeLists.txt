link_libraries(gtest pthread)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

aux_source_directory(. DIR_TESTS)

add_executable(PACK_TEST ${DIR_TESTS})