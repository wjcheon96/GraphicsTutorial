cmake_minimum_required(VERSION 3.27.2)

set(PROJECT_NAME scop)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

# ExternalProject 관련 명령어 셋 추가
include(ExternalProject)

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INSTALL_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})

