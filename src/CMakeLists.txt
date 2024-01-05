##
# @file CMakeLists.txt
# src目录的cmake文件
#
include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(./ SRC_DIR)
set(LINK_DIR ${CMAKE_SOURCE_DIR}/lib)
link_directories(${LINK_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_DIR})
# 设置install规则
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# 链接 pthread库
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
