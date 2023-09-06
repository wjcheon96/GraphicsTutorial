#include "Context.hpp"
#include "Image.hpp"

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
    // vertices가 [x, y, z, r, g, b, s, t]를 지닌다.
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, 
        GL_STATIC_DRAW, vertices, sizeof(float) * 32);

    // 0번 attribute만 쓰는것이 아닌, 1번 attribute도 써서, 0번에 위치, 1번에 색상을 지정하게끔 하려 함.
    // stride가 증가한 이유는, 하나의 점을 표시를 할 때 2개의 attribute를 담기에, 6개가 된다.
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    // s, t(texture의 x, y)가 추가되었으므로, 해당 attribute를 쉐이더에 담아주어야함.
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);
    // m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 
    //     sizeof(float) * 3, 0);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
        GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    // gl function이 생성된 이후에 쉐이더를 호출해서 사용해야함.
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
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

    //이미지 path를 통해서 이미지를 불러온다.
    auto image = Image::Load("./image/container.jpg");
    if (!image) 
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels",
    image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    // opengl texture에 대한 object를 생성해서, 해당 id를 m_texture 안에 집어넣는다.
    glGenTextures(1, &m_textures);
    // m_texture의 id 와 2D texture를 바인딩을 진행한다.
    glBindTexture(GL_TEXTURE_2D, m_textures);
    // texture filter 와 wrapping 과 관련된 인수를 세팅한다.
    // 뒤에 i는 integer(정수)라는 것을 의미한다.
    // TEXTURE_MIN_FILTER는 이미지가 많이 축소 되었을때, TEXTURE_MAG_FILTER는 많이 커졌을때 쓰는 필터이며, 이를 둘 다 linear로 지정해준다.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 0,1 을 넘어가는 값으로 들어올 때 wrapping을 어떻게 할 것이냐.
    // TEXTURE_WRAP_S는 x축, TEXTURE_WRAP_T는 y축 좌표를 의미한다.
    // 이때 어떤 방식으로 wrapping 하는지는 GL_CLAMP_TO_EDGE 방식을 사용한다.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // gpu로 복사하는 과정.
    // 2d texture를 타겟으로 잡고, 타겟과 레벨을 제외하고 앞의 3개 인자는 GPU쪽의 texture 데이터에 대한 내용을 기술.
    // 끝의 3개는 실제 존재하는 cpu쪽의 image의 정보를 명시한다.
    // 0은 level 값으로, 기본 이미지 사이즈를 나타낸다.
    // 5번째 인자인 0은 border size를 나타낸다.
    // GL_UNSIGNED_BYTE는 하나의 채널을 표현하는데 쓰는 데이터 타입이 무엇인지를 명시해준다.
    // 마지막에 이미지 데이터가 담겨있는 포인터를 넘겨준다.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0,
        GL_RGB, GL_UNSIGNED_BYTE, image->GetData());

    return true;
}

void Context::Render() {
    // 실제 framebuffer를 clear함.
    glClear(GL_COLOR_BUFFER_BIT);

    // static으로 선언했기에, 매 프레임마다 반복적으로 실행될때마다 값이 증가하게 된다.
    static float time = 0.0f;
    float t = sinf(time) * 0.5f + 0.5f;
    // 프로그램 인스턴스의 아이디를 가져와서 세팅한다.
    // color라는 string을 집어넣는데, 이는 uniform variable의 변수 명이 된다.
    // return되는 에 glUniform4f()의 인자로 호출을 해서 가져온다.
    // 해당 program을 쓰면서, color 변수를 uniform variable로 지정한다.
    auto loc = glGetUniformLocation(m_program->Get(), "color");
    m_program->Use();
    // 색상 변화를 지정해서 넣어준다.
    glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    time += 0.016f;
}
