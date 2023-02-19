cmake_minimum_required(VERSION 3.10.2)

project(Particle-Life)

add_executable(${PROJECT_NAME} ./src/main.cpp ./src/particle.cpp ./deps/glad.c) 

target_include_directories(${PROJECT_NAME} PUBLIC ./src/include ./deps/include/)

target_link_libraries(${PROJECT_NAME} glfw GL X11 pthread Xrandr Xi dl)
