cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(core)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/core.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/env/env.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/gll/gll.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/input/input.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/logger/logger.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/thread/thread.c
  ${CMAKE_CURRENT_LIST_DIR}/src/core/window/window.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
  ${CMAKE_CURRENT_LIST_DIR}/libs
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY bin
)

# Libraries
set(CORE_LIBS "")

if (UNIX)
  set(CORE_LIBS "-lX11 -lGL -pthread")
endif (UNIX)

if (WIN32)
  set(CORE_LIBS "opengl32")
  add_definitions(-DGLEW_STATIC)
endif (WIN32)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_LIBS})

