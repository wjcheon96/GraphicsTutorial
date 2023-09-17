#include "Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// 이미지 경로를 받아와서 초기화를 진행한 이후, loading에 성공시 해당 포인터를 넘겨줌.
ImageUPtr Image::Load(const std::string& filepath) {
    auto image = ImageUPtr(new Image());
    if (!image->LoadWithStb(filepath))
        return nullptr;
    return std::move(image);
}

ImageUPtr Image::Create(int width, int height, int channelCount) {
    auto image = ImageUPtr(new Image());
    if (!image->Allocate(width, height, channelCount))
        return nullptr;
    return std::move(image);
}

bool Image::Allocate(int width, int height, int channelCount) {
    m_width = width;
    m_height = height;
    m_channelCount = channelCount;
    m_data = (uint8_t *)malloc(m_width * m_height * channelCount);
    return m_data ? true : false;
}

// 체크 무늬 이미지 생성
void Image::SetCheckImage(int gridX, int gridY) {
    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            int pos = (j * m_width + i) * m_channelCount;
            bool even = ((i / gridX) + (j / gridY)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k = 0; k < m_channelCount; k++)
                m_data[pos + k] = value;
            if (m_channelCount > 3)
                m_data[3] = 255;
        }
    }
}


bool Image::LoadWithStb(const std::string& filepath) {
    // 보통의 이미지는 좌상단을 원점으로 하나, opengl에서는 좌하단을 원점으로 하기에, 이미지 로딩시 상하를 반전시켜 문제를 해결할 수 있다.
    stbi_set_flip_vertically_on_load(true);
    // stbi_load 함수를 호출한다. 인자로 char *를 받으므로, c_str()로 변환하며, return 되는 값들을 지정해준다.
    // 연산이 성공시 m_data에 이미지 데이터의 주소값이 담긴다.
    m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0);
    if (!m_data) {
        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }
    return true;
}

Image::~Image() {
    if (m_data) {
        stbi_image_free(m_data);
    }
}

// 단색 texture를 만든다.
ImageUPtr Image::CreateSingleColorImage(int width, int height, const glm::vec4& color) {
    // color를 uint8_t로 바꾸면서, color가 0 ~ 1 사이의 값으로 들어오므로, 255를 곱해서 0 ~ 255 사이의 값으로 만들어준다.
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
    uint8_t rgba[4] = {
        (uint8_t)clamped.r, 
        (uint8_t)clamped.g, 
        (uint8_t)clamped.b, 
        (uint8_t)clamped.a, 
    };
    // 4개의 채널을 가진 이미지를 만들어, 4byte씩 복사한다.
    auto image = Create(width, height, 4);
    for (int i = 0; i < width * height; i++) {
        memcpy(image->m_data + 4 * i, rgba, 4);
    }
    return std::move(image);
}
