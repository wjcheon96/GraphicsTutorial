cmake_minimum_required(VERSION 3.27.3)

project(scop)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCES
    srcs/main.cpp
    srcs/glad.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    libglfw.3.dylib
    # -framework Cocoa
    # -framework OpenGL
    # -framework IOKit
)
