cmake_minimum_required(VERSION 3.16)
project(MyGameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recommended build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect project sources
file(GLOB_RECURSE PROJECT_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(MyGameEngine ${PROJECT_SOURCES})

# Include directories used by the project
# Conan packages provide include dirs and targets.
target_include_directories(MyGameEngine PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/libs/raylib/include
)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(imgui REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(sol2 REQUIRED CONFIG)
find_package(lua 5.4 REQUIRED CONFIG)
find_package(box2d REQUIRED CONFIG)
find_package(GTest REQUIRED CONFIG)

# Import raylib library
add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
  IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/libs/raylib/lib/raylib.lib"
  IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/libs/raylib/lib/raylib.lib"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libs/raylib/include"
)

# Link libraries provided by Conan
if(TARGET SDL2::SDL2)
  target_link_libraries(MyGameEngine PRIVATE SDL2::SDL2 SDL2::SDL2main)
endif()
target_link_libraries(MyGameEngine PRIVATE
  SDL2_image::SDL2_image
  sdl_ttf::sdl_ttf
  SDL2_mixer::SDL2_mixer
  imgui::imgui
  glm::glm
  sol2::sol2
  lua::lua
  box2d::box2d
  raylib
  winmm
  ws2_32
)

# Platform-specific settings
if(MSVC)
  target_compile_options(MyGameEngine PRIVATE /W4 /permissive-)
else()
  target_compile_options(MyGameEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Sources: ${PROJECT_SOURCES}")

# ========================================
# Google Test Configuration
# ========================================
enable_testing()

# Collect test sources
file(GLOB_RECURSE TEST_SOURCES
  ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

message(STATUS "Tests enabled. Found ${CMAKE_SOURCE_DIR}/tests/*.cpp")
message(STATUS "Test sources: ${TEST_SOURCES}")

if(TEST_SOURCES)
  # Adicionar implementações necessárias
  set(TEST_IMPL_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Logger/Logger.cpp
    ${CMAKE_SOURCE_DIR}/src/ECS/ESC.cpp
  )
  
  add_executable(MyGameEngine_tests ${TEST_SOURCES} ${TEST_IMPL_SOURCES})
  
  target_include_directories(MyGameEngine_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/raylib/include
  )
  
  target_link_libraries(MyGameEngine_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    box2d::box2d
    glm::glm
    lua::lua
    sol2::sol2
    SDL2::SDL2
    SDL2::SDL2main
  )
  
  # Discover and register tests
  include(GoogleTest)
  gtest_discover_tests(MyGameEngine_tests)
else()
  message(STATUS "No test files found in tests/ directory")
endif()
