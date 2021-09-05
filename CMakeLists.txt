cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_PROJECT_TYPE Debug)

project(core)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/src/core.c
  ${CMAKE_CURRENT_LIST_DIR}/src/driver.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC X11)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY bin
)
