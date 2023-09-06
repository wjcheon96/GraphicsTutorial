#include "Texture.hpp"

TextureUPtr Texture::CreateFromImage(const Image* image) {
    auto texture = TextureUPtr(new Texture());
    texture->CreateTexture();
    texture->SetTextureFromImage(image);
    return std::move(texture);
}

Texture::~Texture() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }
}

// m_texture의 id 와 2D texture를 바인딩을 진행한다.
void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

// texture filter 와 wrapping 과 관련된 인수를 세팅한다.
// 뒤에 i는 integer(정수)라는 것을 의미한다.
// TEXTURE_MIN_FILTER는 이미지가 많이 축소 되었을때, TEXTURE_MAG_FILTER는 많이 커졌을때 쓰는 필터이며, 이를 둘 다 linear로 지정해준다.
void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

// 0,1 을 넘어가는 값으로 들어올 때 wrapping을 어떻게 할 것이냐.
// TEXTURE_WRAP_S는 x축, TEXTURE_WRAP_T는 y축 좌표를 의미한다.
// 이때 어떤 방식으로 wrapping 하는지는 GL_CLAMP_TO_EDGE 방식을 사용한다.
void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}
    
void Texture::CreateTexture() {
    // opengl texture에 대한 object를 생성해서, 해당 id를 m_texture 안에 집어넣는다.
    glGenTextures(1, &m_texture);
    // bind and set default filter and wrap option
    Bind();
    SetFilter(GL_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

// 스마트 포인터가 아닌 Image*를 쓴 이유는 소유권과 상관없이 인스턴스에 접근하기 위함.
// ImageUptr을 사용하게 되면 인스턴스 소유권이 넘어오므로, 내부에서 그 인스턴스를 받아와서 저장해야함.
// 이 경우에는 소유권이 넘어와서 외부에서 사용이 불가함.
// ImagePtr을 사용시에는 인스턴스 소유권을 공유하는데, 실질적으로 이미지 데이터를 gpu에 넘겨줘서 정보를 전달하는 목적으로만 사용하기에,
// 소유권을 가지고 있을 이유가 없어 *로 사용함.
void Texture::SetTextureFromImage(const Image* image) {
    GLenum format = GL_RGBA;
    switch (image->GetChannelCount()) {
        default: break;
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
    }
    
    // gpu로 복사하는 과정.
    // 2d texture를 타겟으로 잡고, 타겟과 레벨을 제외하고 앞의 3개 인자는 GPU쪽의 texture 데이터에 대한 내용을 기술.
    // 끝의 3개는 실제 존재하는 cpu쪽의 image의 정보를 명시한다.
    // 0은 level 값으로, 기본 이미지 사이즈를 나타낸다.
    // 5번째 인자인 0은 border size를 나타낸다.
    // GL_UNSIGNED_BYTE는 하나의 채널을 표현하는데 쓰는 데이터 타입이 무엇인지를 명시해준다.
    // 마지막에 이미지 데이터가 담겨있는 포인터를 넘겨준다.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        image->GetWidth(), image->GetHeight(), 0,
        format, GL_UNSIGNED_BYTE,
        image->GetData());
}
