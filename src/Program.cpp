#include "Program.hpp"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
    auto program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
        return nullptr;
    return std::move(program);
}

ProgramUPtr Program::Create(const std::string& vertShaderFilename, const std::string& fragShaderFilename) {
    ShaderPtr vs = Shader::CreateFromFile(vertShaderFilename, GL_VERTEX_SHADER);
    ShaderPtr fs = Shader::CreateFromFile(fragShaderFilename, GL_FRAGMENT_SHADER);
    if (!vs || !fs)
        return nullptr;
    return std::move(Create({vs, fs}));
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

void Program::Use() const {
    glUseProgram(m_program);
}

// 함수 오버로딩. uniform location만 받으면 int 값을 세팅하고, matrix 값을 밭으면 아래 함수를 통해서 세팅한다.

void Program::SetUniform(const std::string& name, int value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(loc, value);
}

void Program::SetUniform(const std::string& name, float value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string& name, const glm::vec3& value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::mat4& value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    // matrix 값 세팅하는데 필요한 함수로, location, 몇개가 들어가는지, transpose여부, transform class가 저장하고 있는 floating point의 가장 첫번째 주소의 주소값을 리턴한다.
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::vec4& value) const {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}