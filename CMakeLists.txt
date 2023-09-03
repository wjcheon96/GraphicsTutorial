# CMake 최소 버전 명시.
cmake_minimum_required(VERSION 3.27.2)

# 프로젝트 이름 및 버전 명시
set(PROJECT_NAME scop)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 환경 변수 세팅
set(WINDOW_NAME "scop")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 640)

# Mac에서 돌릴때 필요한 추가적인 Flag들.
if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -Wall -Wextra")
endif()

# 프로젝트 이름 설정
project(${PROJECT_NAME})
# 실행파일의 이름 설정
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/Common.cpp src/Common.hpp
    src/Shader.cpp src/Shader.hpp
    src/Program.cpp src/Program.hpp
    src/Context.cpp src/Context.hpp
    src/Buffer.cpp src/Buffer.hpp
    src/VertexLayout.cpp src/VertexLayout.hpp
    )

# ExternalProject 관련 명령어 셋 추가
include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# define 매크로로 지정한 값을 실제 컴파일 과정에서 지정
target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})
