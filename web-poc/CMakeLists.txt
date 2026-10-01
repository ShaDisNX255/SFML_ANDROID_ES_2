cmake_minimum_required(VERSION 3.12)

project(SFMLWebCanvasPoC LANGUAGES C CXX)

if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "This test must be configured with emcmake / Emscripten.")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build only the pieces needed for the first browser canvas test.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

message(STATUS "[ONB_WEB_BOOT] Configuring standalone SFML canvas test")

# Build the parent SFML source tree inside this test's build directory.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/sfml)

add_executable(sfml-web-canvas
    main.cpp
)

target_link_libraries(sfml-web-canvas PRIVATE
    sfml-window
    sfml-system
)

# .html output asks Emscripten to generate the HTML launcher plus JS/WASM.
set_target_properties(sfml-web-canvas PROPERTIES
    OUTPUT_NAME "sfml-web-canvas"
    SUFFIX ".html"
)

target_link_options(sfml-web-canvas PRIVATE
    "SHELL:-s ASSERTIONS=2"
    "SHELL:-s GL_ASSERTIONS=1"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
)

message(STATUS "[ONB_WEB_BOOT] Browser output target: sfml-web-canvas.html")
