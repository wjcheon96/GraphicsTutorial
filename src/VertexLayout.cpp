#include "VertexLayout.hpp"

VertexLayoutUPtr VertexLayout::Create() {
    auto vertexLayout = VertexLayoutUPtr(new VertexLayout());
    vertexLayout->Init();
    return std::move(vertexLayout);
}

VertexLayout::~VertexLayout() {
    if (m_vertexArrayObject) {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }
}

void VertexLayout::Bind() const {
    glBindVertexArray(m_vertexArrayObject);
}

void VertexLayout::SetAttrib(uint32_t attribIndex, int count,
    uint32_t type, bool normalized, size_t stride, uint64_t offset) const {
    // 인자로 들어가는attributeIndex에 해당하는 번호의 attribute를 쓸거다는 것을 명시함.
    glEnableVertexAttribArray(attribIndex);
    // attribute index부터 count 개수만큼 floating point를 주고, type이 어떻게 되며, normalize 여부, stride 갯수, offset이 어디인지 등을 지정함.
    glVertexAttribPointer(attribIndex, count,
        type, normalized, stride, (const void*)offset);
}

void VertexLayout::Init() {
    // VBO를 생성하기 전에 VAO를 세팅한다.
    // vertexArrayObject와 이 object가 사용할 buffer를 같이 세팅을 하게끔 한다.
    glGenVertexArrays(1, &m_vertexArrayObject);
    Bind();
}
