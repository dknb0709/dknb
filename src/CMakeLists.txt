add_library(thread_pool
  SHARED
    thread_pool.cpp
  )
target_include_directories(thread_pool
    PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

