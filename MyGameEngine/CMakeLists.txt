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
)

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(imgui REQUIRED CONFIG)
find_package(glm REQUIRED CONFIG)
find_package(sol2 REQUIRED CONFIG)
find_package(lua 5.4 REQUIRED CONFIG)

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
)

# Platform-specific settings
if(MSVC)
  target_compile_options(MyGameEngine PRIVATE /W4 /permissive-)
else()
  target_compile_options(MyGameEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Sources: ${PROJECT_SOURCES}")
