add_library(sample
  SHARED
    sample.cpp
  )
target_include_directories(sample
    PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

add_library(sample2
  SHARED
    sample2.cpp
  )
target_include_directories(sample2
    PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

