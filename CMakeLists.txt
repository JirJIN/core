cmake_minimum_required(VERSION 3.10)

# Global Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(core)

set(DRIVER "${CMAKE_CURRENT_LIST_DIR}/src/driver.c")
if (EMSCRIPTEN)
  set(DRIVER "${CMAKE_CURRENT_LIST_DIR}/src/driver_web.c")
endif ()

add_executable(${PROJECT_NAME}
  ${DRIVER}
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
  # Windows include directory
  C:/cpplib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY bin
)

# Libraries
set(CORE_LIBS "")

if (EMSCRIPTEN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s OFFSCREEN_FRAMEBUFFER=1 -s USE_WEBGL2=1")

elseif (UNIX)
  set(CORE_LIBS "-lX11 -lGL -pthread")

elseif (WIN32)
  set(CORE_LIBS "opengl32")

endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC ${CORE_LIBS})

