cmake_minimum_required(VERSION 3.12)
project(opengl_10)

set(CMAKE_CXX_STANDARD 11)
set(HEADERS /usr/local/include)
set(HEADERS2 ./src)
set(GLFW_LINK /usr/local/lib/libglfw3.a)
set(ASSIMP_LINK /usr/local/lib/libassimp.dylib)
set(FRAMEWORKS_1 /System/Library/Frameworks/Cocoa.framework)
set(FRAMEWORKS_2 /System/Library/Frameworks/OpenGL.framework)
set(FRAMEWORKS_3 /System/Library/Frameworks/GLUT.framework)
set(FRAMEWORKS_4 /System/Library/Frameworks/CoreVideo.framework)
set(FRAMEWORKS_5 /System/Library/Frameworks/IOKit.framework)

include_directories(${HEADERS} ${HEADERS2})

link_libraries(${GLFW_LINK} ${ASSIMP_LINK} ${FRAMEWORKS_1} ${FRAMEWORKS_2} ${FRAMEWORKS_3} ${FRAMEWORKS_4} ${FRAMEWORKS_5})

add_executable(opengl_10 main.cpp src/glad.c src/mesh.h src/shader.h src/camera.h src/model.h)