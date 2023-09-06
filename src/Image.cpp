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

Image::~Image() {
    if (m_data) {
        stbi_image_free(m_data);
    }
}

bool Image::LoadWithStb(const std::string& filepath) {
    // stbi_load 함수를 호출한다. 인자로 char *를 받으므로, c_str()로 변환하며, return 되는 값들을 지정해준다.
    // 연산이 성공시 m_data에 이미지 데이터의 주소값이 담긴다.
    m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0);
    if (!m_data) {
        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }
    return true;
}
