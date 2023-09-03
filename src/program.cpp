#include "program.hpp"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
    auto program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
        return nullptr;
    return std::move(program);
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) {
    // glCreateShader() 와 마찬가지로 id를 하나 얻어서 초기화를 진행한다.
    m_program = glCreateProgram();
    for (auto& shader: shaders)
        // glAttachShader 함수를 통해서 인자로 들어온 shader들을 하나씩 setting함.
        glAttachShader(m_program, shader->Get());
    // vertex shader와 fragment shader가 attach 되어있는 상황에서 링킹을 진행함.
    glLinkProgram(m_program);

    int success = 0;
    // success로 링크가 제대로 진행되는지를 확인함.
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false ;
    }
    return true ;
}

Program::~Program() {
    if (m_program) {
        // m_program이 0이 아니면 무언가 있는 상태이므로, 해당 프로그램을 지움.
        glDeleteProgram(m_program);
    }
}