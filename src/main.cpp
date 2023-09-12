#include "Common.hpp"
#include "Context.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#ifdef __APPLE__
    extern "C" {
        float getScaleFactor(); // Objective-C++에서 정의한 함수를 선언
    }
#endif

// window의 크기가 바꼈을때 해야할 작업 수행.
void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    // glViewport를 통해 opengl이 그릴 영역을 지정해줌.
    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    context->Reshape(width, height);
}

//window 내의 커서 position을 계산하는 함수.
void OnCursorPos(GLFWwindow* window, double x, double y) {
    auto context = (Context*)glfwGetWindowUserPointer(window);
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
    // 마우스 버튼이 클릭되었을때, ImGui화면에 버튼 클릭이 값이 들어가게 한다.
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
    auto context = (Context*)glfwGetWindowUserPointer(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
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

// 텍스트 입력에 대한 콜백함수
void OnCharEvent(GLFWwindow* window, unsigned int ch) {
    ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

int main(int ac, char **av) {
    #ifdef __APPLE__
        float scaleFactor = getScaleFactor();
    #else
        float scaleFactor = 1.0f;
    #endif

    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = NULL;
        glfwGetError(&description);
        SPDLOG_ERROR("Failed to initialize glfw: {}", description);
        return -1;
    }

    // init 이후 window 생성 전에 만들기 희망하는 OpenGL의 버전을 추가해줌.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    SPDLOG_INFO("Create glfw window");
    // glfwCreateWindow를 호출 시 window가 생성되며, 동시에 window에서 사용되는 openGL context가 만들어진다.
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("Failed to create glfw window");
        glfwTerminate();
        return -1;
    }

    // 따라서 glfwCreateWindow에서 생성된 openGL context를 사용하게끔 명시적으로 넣어줘야 한다.
    glfwMakeContextCurrent(window);

    // context 생성 이후에 glad 라이브러리를 통해 openGL함수를 로딩한다.
    // process address를 얻어오는 함수를 통해, gl 함수를 로딩함.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("Failed to initialize glad");
        glfwTerminate();
        return -1;
    }

    // 정상적으로 gl함수가 잘 불려왔는지를 확인한다.
    // opengl function 중에서 가장 기본적인 정보를 가져오는 glGetString을 통해 확인해본다.
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", (char *)glVersion);

    // imgui의 context를 생성한다.
    auto imguiContext = ImGui::CreateContext();
    // 위에서 생성한 imgui context를 현재 context로 세팅을 한다.
    ImGui::SetCurrentContext(imguiContext);
    // imple_glfw 내부의 initForOpenGl 함수를 호출하는데, window를 잡고, 콜백에 대한 세팅을 직접 진행하기에, 자동으로 콜백하는 것을 막아준다.
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    // opengl3.3 version의 렌더러를 초기화한다. 
    ImGui_ImplOpenGL3_Init();
    // imgui가 사용하고자 하는 font나 object(쉐이더프로그램, 텍스처 등)를 초기화를 진행한다.
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    // shader와 program을 context 객체를 통해서 관리.
    // unique_ptr의 reset 함수를 통해 메모리 관리를 용이하게 함.
    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    // resizing시, mac 의 경우 픽셀을 1단위픽셀을 2배로 잡기에 생기는 문제를 해결하기 위함.
    glfwSetWindowUserPointer(window, context.get());
    SPDLOG_INFO("scale factor: {}", scaleFactor);
    OnFramebufferSizeChange(window, WINDOW_WIDTH * scaleFactor, WINDOW_HEIGHT * scaleFactor);

    // glfw로 생성된 윈도우에 특정 이벤트 발생시 실행되는 콜백함수 지정.
    // 아래와 같이 glfwXXXCallback의 형태를 가짐.
    // window와 콜백 함수를 매개변수로 집어넣는다.
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    glfwSetCharCallback(window, OnCharEvent);
    glfwSetScrollCallback(window, OnScrollCallback);
    glfwSetCursorPosCallback(window, OnCursorPos);
    glfwSetMouseButtonCallback(window, OnMouseButton);

    while (!glfwWindowShouldClose(window)) {
        // 아래 루프문에서 event가 발생시 해당 event를 수집함.
        glfwPollEvents();

        // ImGui가 매 프레임마다 새롭게 계산된다.
        ImGui_ImplGlfw_NewFrame();
        // 해당 새 프레임의 생성을 명시적으로 알려준다.
        ImGui::NewFrame();
        
        context->ProcessInput(window);
        context->Render();

        // 실제 렌더 위에 그린다. 
        // Render() 함수는 실제로 그리진 않고, 이후에 렌더를 할 리스트를 종합하는 함수이다.
        ImGui::Render();
        // 실제 opengl3 function을 이용해서 그리는 함수. GetDrawData()에서 실제로 그린다.
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    // 종료 전에 ImGUI 관련 스택을 없애준다.
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    glfwTerminate();
    return 0;
}
