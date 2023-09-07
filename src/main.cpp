#include "Context.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// #include <spdlog/spdlog.h>
// // glad는 반드시 glfw 이전에 헤더 명시를 해주어야한다.
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>


// window의 크기가 바꼈을때 해야할 작업 수행.
void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    // glViewport를 통해 opengl이 그릴 영역을 지정해줌.
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key:{}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed":
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? " Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int ac, char **av) {
    std::cout << "Initialize glfw" << std::endl;
    if (!glfwInit()) {
        const char* description = NULL;
        glfwGetError(&description);
        std::cout << "Failed to initialize glfw: " << description << std::endl;
        return -1;
    }

    // init 이후 window 생성 전에 만들기 희망하는 OpenGL의 버전을 추가해줌.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    std::cout << "Create glfw window" << std::endl;
    // glfwCreateWindow를 호출 시 window가 생성되며, 동시에 window에서 사용되는 openGL context가 만들어진다.
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        std::cout << "failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 따라서 glfwCreateWindow에서 생성된 openGL context를 사용하게끔 명시적으로 넣어줘야 한다.
    glfwMakeContextCurrent(window);

    // glfw로 생성된 윈도우에 특정 이벤트 발생시 실행되는 콜백함수 지정.
    // 아래와 같이 glfwXXXCallback의 형태를 가짐.
    // window와 콜백 함수를 매개변수로 집어넣는다.
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // context 생성 이후에 glad 라이브러리를 통해 openGL함수를 로딩한다.
    // process address를 얻어오는 함수를 통해, gl 함수를 로딩함.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to initialize glad" << std::endl;
        glfwTerminate();
        return -1;
    } 

    // 정상적으로 gl함수가 잘 불려왔는지를 확인한다.
    // opengl function 중에서 가장 기본적인 정보를 가져오는 glGetString을 통해 확인해본다.
    auto glVersion = glGetString(GL_VERSION);
    std::cout << "OpenGl context version: " << glVersion << std::endl;

    // shader와 program을 context 객체를 통해서 관리.
    // unique_ptr의 reset 함수를 통해 메모리 관리를 용이하게 함.
    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        // 아래 루프문에서 event가 발생시 해당 event를 수집함.
        glfwPollEvents();
        context->Render();
        /*
            Framebuffer swap은 화면에 그림을 그리는 과정으로,
            프레임 버퍼를 2개를 준비하여(front / back)
            front buffer에 바로 그리게 되면 그림이 그려지는 과정까지 보여 더러워지기에, 
            back buffer에 그림을 그리고, front와 back을 바꿔치기 하면서 위 과정을 반복하여
            그림이 그려지는 과정이 노출되지 않게 함.
            double buffering이라고 함.
        */
        glfwSwapBuffers(window);
    }
    // std::unique_ptr의 reset() 함수를 통해 메모리를 정리.
    context.reset();

    glfwTerminate();
    return 0;
}
