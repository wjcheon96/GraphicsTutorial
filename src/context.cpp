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
    // 정점 데이터를 담은 array를 선언.
    // openGL화면은 가로 세로가 -1 ~ +1로 normalizing 된 좌표계를 사용한다.
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, // top left
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };


    // VBO를 생성하기 전에 VAO를 세팅한다.
    // vertexArrayObject와 이 object가 사용할 buffer를 같이 세팅을 하게끔 한다.
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // vertex buffer를 생성한다.
    // 해당 버퍼(m_vertexBuffer)는 크게 2가지 용도로 쓰는데, element(index) buffer object 혹은 vertex buffer object의 용도로 사용된다.
    // GL_ARRAY_BUFFER는 vertex buffer를 나타낸다.
    glGenBuffers(1, &m_vertexBuffer);
    // 데이터를 복사해 넣기 위해서 bind를 먼저 해줘야한다.
    // 사용할 buffer object가 vertex data임을 알려줌.
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    // 실제 데이터를 복사. 데이터의 byte 사이즈와 데이터를 복사함. 
    // GL_STATIC_DRAW를 통해 버퍼를 변경하지 않겠다는 내용을 알려줌.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

    // 0번 attribute를 쓸거다는 것을 명시함.
    glEnableVertexAttribArray(0);
    // floating point 값은 3개, 각각 float값이며, normalizing 여부, stride는 3개씩, offset은 어디인지 등의 자료를 담는다.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

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

    // window를 clear할때 clear할 색상 지정.
    glClearColor(0.0f, 0.1f, 0.2f, 0.0f);
    return true;
}

void Context::Render() {
    // 실제 framebuffer를 clear함.
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program->Get());
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}