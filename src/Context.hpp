#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include "Common.hpp"
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
        void Reshape(int width, int height);
        void MouseMove(double x, double y);
        void MouseButton(int button, int action, double x, double y);
        void MouseScroll(double yoffset);

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
        int m_width { WINDOW_WIDTH };
        int m_height { WINDOW_HEIGHT };

        glm::vec4 m_clearColor { glm::vec4(0.3f, 0.2f, 0.1f, 0.0f) };
        
        glm::vec3 m_lightColor { glm::vec3(1.0f, 1.0f, 1.0f) };
        glm::vec3 m_objectColor { glm::vec3(1.0f, 0.5f, 0.0f) };
        float m_ambientStrength { 1.0f };
    
        // camera parameter.
        // pitch 와 yaw에 대한 값 세팅.
        float m_cameraPitch { 0.0f };
        float m_cameraYaw { 0.0f };
        
        bool m_cameraControl { false };
        glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

};

#endif
