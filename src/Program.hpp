#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include "Common.hpp"
#include "Shader.hpp"

CLASS_PTR(Program)
class Program {
    public:
        // shader와 마찬가지로 create 함수를 통해서 관리.
        // vertex, fragment 쉐이더가 default이나, 이 외의 여러개의 shader를 링크할 수 있어야 함.
        // reference 및 sharedpointer 형태를 사용하여(ShaderPtr) 다른 프로그램을 만드는데 재사용이 가능하게끔 함.
        static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);
        // shader Filename을 받아서 내부에서 쉐이더를 로딩하고 call을 하는 식으로 처리할 수 있게끔 함.
        static ProgramUPtr Create(const std::string& vertShaderFilename, const std::string& fragShaderFilename);
        ~Program();
        uint32_t Get() const { return m_program; }
        void Use() const;

        // uniform 값을 설정하는 메소드
        void SetUniform(const std::string& name, int value) const;
        void SetUniform(const std::string& name, float value) const;
        void SetUniform(const std::string& name, const glm::vec3& value) const;
        void SetUniform(const std::string& name, const glm::vec4& value) const;
        void SetUniform(const std::string& name, const glm::mat4& value) const;
        void SetUniform(const std::string& name, const glm::vec2& value) const;

    private:
        Program() {}
        // 초기화를 하는데 쓰일 link 함수.
        bool Link(const std::vector<ShaderPtr>& shaders);
        uint32_t m_program { 0 };
};

#endif
