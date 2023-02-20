cmake_minimum_required(VERSION 3.10.2)

project(Particle-Life)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(./deps/glfw/)

add_executable(${PROJECT_NAME} ./src/main.cpp ./src/particle.cpp ./deps/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC ./src/include ./deps/include/ ./deps/glfw/include)

target_link_libraries(${PROJECT_NAME} glfw) # GL X11 pthread Xrandr Xi dl)
