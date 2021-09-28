cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(core)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/core.c
  ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
  ${CMAKE_CURRENT_LIST_DIR}/src/env/env.c
  ${CMAKE_CURRENT_LIST_DIR}/src/input/input.c
  ${CMAKE_CURRENT_LIST_DIR}/src/thread/thread.c
  ${CMAKE_CURRENT_LIST_DIR}/src/window/window.c

  # Libraries
  ${CMAKE_CURRENT_LIST_DIR}/libs/glew/glew.c
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
endif (WIN32)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_LIBS})

