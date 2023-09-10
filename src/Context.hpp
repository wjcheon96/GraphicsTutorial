#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include "Common.hpp"
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "Program.hpp"
#include "Buffer.hpp"
#include "VertexLayout.hpp"
#include "Texture.hpp"

CLASS_PTR(Context)
class Context {
    public:
        static ContextUPtr Create();
        void Render();
        void ProcessInput(GLFWwindow* window);
    private:
        Context() {}
        bool Init();

        ProgramUPtr m_program;
        VertexLayoutUPtr m_vertexLayout;
        BufferUPtr m_vertexBuffer;
        BufferUPtr m_indexBuffer;
        TextureUPtr m_texture;
        TextureUPtr m_texture2;

        glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
        // 바라보는 방향 세팅.
        glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
        // 카메라가 특정 지점을 바라보게 하는 법.
        glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
};

#endif
