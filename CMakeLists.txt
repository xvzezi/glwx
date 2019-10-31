cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME OPENGL_WRAP_X)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

set(GLFW_INC_DIR ../global/include)
set(GLFW_LIB_DIR ../global/lib)
set(X_DIR ./glwx)

include_directories(${GLFW_INC_DIR} ${X_DIR})
link_directories(${GLFW_LIB_DIR})
link_libraries(glfw3)

set(X_MODULE glwx/xworld.cpp glwx/xobject.cpp glwx/xshader.cpp glwx/xcamera.cpp glwx/stb_image.cpp
        glwx/xparticle.cpp
        glwx/glad.c glwx/xline.cpp)

set(PROJ main.cpp prob1_world.h prob1_world.cpp prob1_solver.h prob2_world.h prob2_solver.h force_particle.h arrow.h)

add_executable(${PROJECT_NAME} ${PROJ} ${X_MODULE} ${IMGUI})
target_link_libraries(${PROJECT_NAME} glfw3)