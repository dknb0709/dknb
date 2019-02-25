add_executable(test_thread_pool.out test_thread_pool.cpp)
target_link_libraries(test_thread_pool.out thread_pool pthread)

# header only
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  )
add_executable(test_string_util.out test_string_util.cpp)
add_executable(test_singleton.out test_singleton.cpp)
add_executable(test_property.out test_property.cpp)

