cmake_minimum_required(VERSION 3.14)
project(PING_PONG)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(Main Main.cpp glad/src/glad.c)

target_include_directories(Main PRIVATE glad/include)
target_link_libraries(Main PRIVATE glfw OpenGL::GL ${CMAKE_DL_LIBS})