cmake_minimum_required(VERSION 3.22)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(untitled1 main.cpp src/glad.c include/learnopengl/shader_s.h include/stb_image.h stb_image_wrap.cpp include/learnopengl/shader_m.h include/learnopengl/camera.h)

target_link_libraries(untitled1 ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)
