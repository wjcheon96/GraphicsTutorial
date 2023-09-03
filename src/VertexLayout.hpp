#ifndef __VERTEX_LAYOUT_HPP__
#define __VERTEX_LAYOUT_HPP__

#include "Common.hpp"

// VAO 생성 및 메모리 해제시 VAO 제거
// vertex attribute 설정 기능 제공
CLASS_PTR(VertexLayout)
class VertexLayout {
    public:
        static VertexLayoutUPtr Create();
        ~VertexLayout();

        uint32_t Get() const { return m_vertexArrayObject; }
        void Bind() const;
        void SetAttrib(
            uint32_t attribIndex, int count,
            uint32_t type, bool normalized,
            size_t stride, uint64_t offset) const;
        void DisableAttrib(int attribIndex) const;

    private:
        VertexLayout() {}
        // 실패할 일이 없음.
        void Init();
        uint32_t m_vertexArrayObject { 0 };
};

#endif
