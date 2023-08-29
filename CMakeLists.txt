cmake_minimum_required(VERSION 3.27.2)

set(PROJECT_NAME scop)
set(CMAKE_CXX_STANDARD 17)

# 환경 변수 세팅
set(WINDOW_NAME "scop")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

# if (APPLE)
#     set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL")
# endif()


project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} src/main.cpp)

# ExternalProject 관련 명령어 셋 추가
include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INSTALL_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# define 매크로로 지정한 값을 실제 컴파일 과정에서 지정
target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH="${WINDOW_WIDTH}"
    WINDOW_HEIGHT="${WINDOW_HEIGHT}"
)

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})
