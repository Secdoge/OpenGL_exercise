cmake_minimum_required(VERSION 3.12)
project(opengl_02)

set(CMAKE_CXX_STANDARD 11)
set(HEADERS /usr/local/include)
set(GLFW_LINK /usr/local/lib/libglfw3.a)
set(FRAMEWORKS_1 /System/Library/Frameworks/Cocoa.framework)
set(FRAMEWORKS_2 /System/Library/Frameworks/OpenGL.framework)
set(FRAMEWORKS_3 /System/Library/Frameworks/GLUT.framework)
set(FRAMEWORKS_4 /System/Library/Frameworks/CoreVideo.framework)
set(FRAMEWORKS_5 /System/Library/Frameworks/IOKit.framework)

include_directories(${HEADERS})

link_libraries(${GLFW_LINK} ${FRAMEWORKS_1} ${FRAMEWORKS_2} ${FRAMEWORKS_3} ${FRAMEWORKS_4} ${FRAMEWORKS_5})

add_executable(opengl_02 main.cpp src/glad.c)