#include "Context.hpp"
#include "GLFW/glfw3.h"
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
    // float vertices[] = {
    //     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    // };

    // uint32_t indices[] = { // note that we start from 0!
    //     0, 1, 3, // first triangle
    //     1, 2, 3, // second triangle
    // };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, 
        GL_STATIC_DRAW, vertices, sizeof(float) * 120);

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);


    // 0번 attribute만 쓰는것이 아닌, 1번 attribute도 써서, 0번에 위치, 1번에 색상을 지정하게끔 하려 함.
    // stride가 증가한 이유는, 하나의 점을 표시를 할 때 2개의 attribute를 담기에, 6개가 된다.
    // m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    // m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    // // s, t(texture의 x, y)가 추가되었으므로, 해당 attribute를 쉐이더에 담아주어야함.
    // m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);
    // m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 
    //     sizeof(float) * 3, 0);

    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,
        GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

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
    // auto image = Image::Create(512, 512);
    // image->SetCheckImage(16, 16);
    auto image = Image::Load("./image/container.jpg");
    if (!image)
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels",
        image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    m_texture = Texture::CreateFromImage(image.get());

    auto image2 = Image::Load("./image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    // 사용할 슬롯 번호를 알려준다.
    glActiveTexture(GL_TEXTURE0);
    // 세팅하려는 텍스처의 타입과, 해당 id를 바인딩한다.
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    // 슬롯을 1번으로 바꿔준다.
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use();
    // glGetUniformLocation으로 location 값을 얻어와서, 2번째 인자를 통해 해당 슬롯을 이용하겠다는것을 shader에 알려준다.
	m_program->SetUniform("tex", 0);
    m_program->SetUniform("tex2", 1);

    // x축으로 55도만큼 회전시킨다.
    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // 종횡비 4:3, 세로화각 45도의 원근 투영
    auto projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.01f, 10.0f);
    auto transform = projection * view * model;
    auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");

    m_program->SetUniform("transform", transform);

    return true;
}

void Context::Render() {

    // 여러개의 큐브를 그리기 위해 벡터를 여러개를 생성함.
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    // 실제 framebuffer를 clear함.
    // depth초기화
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // depth test를 켜서, z 버퍼상 뒤에 있는 그림(1에 가까운쪽)을 안 그리게끔 한다.
    glEnable(GL_DEPTH_TEST);

    // m_program->Use();
    // zNear, zFar 파라미터가 어느 지점까지 보이는지를 확인 가능하게 한다.
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f);

    // float angle = glfwGetTime() * glm::pi<float>() * 0.5f;
    // auto x = sinf(angle) * 10.0f;
    // auto z = cosf(angle) * 10.0f;
    // auto cameraPos = glm::vec3(x, 0.0f, z);
    // auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // auto view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    auto cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // auto view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    auto view = glm::lookAt(
      m_cameraPos,
      m_cameraPos + m_cameraFront,
      m_cameraUp);

    // auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // auto model = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 120.0f), glm::vec3(1.0f, 0.5f, 0.0f));
    // auto transform = projection * view * model;
    // m_program->SetUniform("transform", transform);
    // 총 36개의 지점을 가지므로 36개가 필요
    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        //모델 회전을 위한 부분
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::vec3(0.0f, 3.0f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // // static으로 선언했기에, 매 프레임마다 반복적으로 실행될때마다 값이 증가하게 된다.
    // static float time = 0.0f;
    // float t = sinf(time) * 0.5f + 0.5f;
    // // 프로그램 인스턴스의 아이디를 가져와서 세팅한다.
    // // color라는 string을 집어넣는데, 이는 uniform variable의 변수 명이 된다.
    // // return되는 에 glUniform4f()의 인자로 호출을 해서 가져온다.
    // // 해당 program을 쓰면서, color 변수를 uniform variable로 지정한다.
    // auto loc = glGetUniformLocation(m_program->Get(), "color");
    // m_program->Use();
    // // 색상 변화를 지정해서 넣어준다.
    // glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // time += 0.016f;
}

void Context::ProcessInput(GLFWwindow* window) {
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraLeft = glm::normalize(glm::cross(m_cameraUp, m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraLeft;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraLeft;

    auto cameraUp = glm::normalize(glm::cross(m_cameraFront, cameraLeft));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraUp;
}
