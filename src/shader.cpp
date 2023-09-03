#include "shader.hpp"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader);
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
    auto result = LoadTextFile(filename);
    // 로딩 실패시 false 리턴
    if (!result.has_value())
        return false;

    // 성공시 로딩된 텍스트 포인터 및 길이 가져오기
    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // glCreateShader()를 통해서 shader 오브젝트 생성
    // 생성시 glCreateShader()는 정수를 반환함.
    m_shader = glCreateShader(shaderType);
    // glShaderSource()로 소스코드 입력
    // 현재 코드는 1개, 들어간 포인터의 주소, 그리고 길이를 넣어준다.
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
    // glCompileShader()로 shader 컴파일
    glCompileShader(m_shader);

      // check compile error
    int success = 0;
    // glGetShaderiv()로 컴파일 상태 조회
    // 실패시 glGetShaderInfoLog()로 에러 로그 가져오기.
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }
    return true;
}

// 최초에 m_shader는 0으로 초기화 되어있기에, 그 외의 다른 값이 들어가있다면 내부 데이터를 지워줘야함.
// 따라서 glDeleteSahder로 shader object를 없애줌.
Shader::~Shader() {
  if (m_shader) {
    glDeleteShader(m_shader);
  }
}

