cmake_minimum_required(VERSION 2.8)

project(bc39)

# debug configuration by default
if (NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# all *.h && *.c && *.cc files in src
file(GLOB_RECURSE sources
  "*.h"
  "*.cc"
)

# avoid generating executables in bin/Debug & bin/Release
if (MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTDIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTDIR})
endif()

# add target
add_library(${PROJECT_NAME} STATIC ${sources})

# configure executable names
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_DEBUG "${PROJECT_NAME}_d")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_RELEASE "${PROJECT_NAME}")

# set additional compiler & linker flags
if (MSVC)
  # treat warnings as errors
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/WX")
endif()
