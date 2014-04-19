cmake_minimum_required(VERSION 2.8.11)

project(bc39example)
set(OUTDIR ${CMAKE_SOURCE_DIR}/bin)

# debug configuration by default
if (NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# header search path for bc39.h
include_directories(../src)

# all *.h && *.c && *.cc files in src
file(GLOB_RECURSE sources
  "*.h"
  "*.cc"
)

# Qt5 forms
file(GLOB_RECURSE uis
  "*.ui"
)

# Qt5 stuffs
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
qt5_wrap_ui(uis ${uis})

# avoid generating executables in bin/Debug & bin/Release
if (MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTDIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTDIR})
endif()

# add target
add_executable(${PROJECT_NAME} ${sources} ${uis})

# link with Qt5Widgets & bc39
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} bc39)

# configure executable names
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_DEBUG "${PROJECT_NAME}_d")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME_RELEASE "${PROJECT_NAME}")

# set additional compiler & linker flags
if (MSVC)
  # treat warnings as errors
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/WX")
endif()
