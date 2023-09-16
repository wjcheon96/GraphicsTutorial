#include "Buffer.hpp"

BufferUPtr Buffer::CreateWithData(uint32_t bufferType, uint32_t usage,
    const void* data, size_t stride, size_t count) {
    auto buffer = BufferUPtr(new Buffer());
    if (!buffer->Init(bufferType, usage, data, stride, count))
        return nullptr;
    return std::move(buffer);
}

Buffer::~Buffer() {
    if (m_buffer) {
        glDeleteBuffers(1, &m_buffer);
    }
}

// 데이터를 복사해 넣기 위해서 bind를 먼저 해줘야한다.
// 사용할 buffer object가 vertex data임을 알려줌.
void Buffer::Bind() const {
    glBindBuffer(m_bufferType, m_buffer);
}

bool Buffer::Init(uint32_t bufferType, uint32_t usage,
    const void* data, size_t stride, size_t count) {
    m_bufferType = bufferType;
    m_usage = usage;
    m_stride = stride;
    m_count = count;
    // vertex buffer를 생성한다.
    // 해당 버퍼(m_vertexBuffer)는 크게 2가지 용도로 쓰는데, element(index) buffer object 혹은 vertex buffer object의 용도로 사용된다.
    // GL_ARRAY_BUFFER는 vertex buffer를 나타낸다.
    glGenBuffers(1, &m_buffer);
    Bind();
    // 실제 데이터를 복사. 데이터의 byte 사이즈와 데이터를 복사함.
    // GL_STATIC_DRAW를 통해 버퍼를 변경하지 않겠다는 내용을 알려줌.
    glBufferData(m_bufferType, m_stride * m_count, data, usage);
    return true;
}
