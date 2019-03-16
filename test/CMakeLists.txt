add_executable(test_thread_pool.out test_thread_pool.cpp)
target_link_libraries(test_thread_pool.out thread_pool pthread)

# header only
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  )
add_executable(test_string_util.out test_string_util.cpp)
add_executable(test_singleton.out test_singleton.cpp)
add_executable(test_property.out test_property.cpp)
set(TEST_FILE_LINKER_FLAGS "-lstdc++fs")
add_executable(test_file.out test_file.cpp)
target_link_libraries(test_file.out ${TEST_FILE_LINKER_FLAGS})
add_executable(test_connection.out test_connection.cpp)

