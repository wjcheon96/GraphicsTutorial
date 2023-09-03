#include "context.hpp"
#include <_types/_uint32_t.h>

// 이전의 program이랑 shader와 거의 흡사한 구조. context를 생성한다.
ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

// program과 shader만든 내용을 init에서 초기화해서 사용함.
bool Context::Init() {
    // gl function이 생성된 이후에 쉐이더를 호출해서 사용해야함.
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    // program의 인자에는 sharedPtr로 들어가야하므로, 이에 맞춰서 설정하고, program을 생성함.
    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());

    uint32_t vao = 0;
    // vertex array 생성
    glGenVertexArrays(1, &vao);
    // vertex array 바인딩
    glBindVertexArray(vao);

    // window를 clear할때 clear할 색상 지정.
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
    return true;
}

void Context::Render() {
    // 실제 framebuffer를 clear함.
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program->Get());
    glDrawArrays(GL_POINTS, 0, 1);
}