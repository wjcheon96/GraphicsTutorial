#include "Context.hpp"

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
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right, red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right, green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left, blue
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // top left, yellow
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, 
        GL_STATIC_DRAW, vertices, sizeof(float) * 24);

    // 0번 attribute만 쓰는것이 아닌, 1번 attribute도 써서, 0번에 위치, 1번에 색상을 지정하게끔 하려 함.
    // stride가 증가한 이유는, 하나의 점을 표시를 할 때 2개의 attribute를 담기에, 6개가 된다.
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
    // m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 
    //     sizeof(float) * 3, 0);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
        GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    // gl function이 생성된 이후에 쉐이더를 호출해서 사용해야함.
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/per_vertex_color.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/per_vertex_color.fs", GL_FRAGMENT_SHADER);
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

    // // static으로 선언했기에, 매 프레임마다 반복적으로 실행될때마다 값이 증가하게 된다.
    // static float time = 0.0f;
    // float t = sinf(time) * 0.5f + 0.5f;
    // // 프로그램 인스턴스의 아이디를 가져와서 세팅한다.
    // // color라는 string을 집어넣는데, 이는 uniform variable의 변수 명이 된다.
    // // return되는 에 glUniform4f()의 인자로 호출을 해서 가져온다.
    // // 해당 program을 쓰면서, color 변수를 uniform variable로 지정한다.
    // auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    // // 색상 변화를 지정해서 넣어준다.
    // glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // time += 0.016f;
}
