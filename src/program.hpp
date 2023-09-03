#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include "common.hpp"
#include "shader.hpp"

CLASS_PTR(Program)
class Program {
    public:
        // shader와 마찬가지로 create 함수를 통해서 관리.
        // vertex, fragment 쉐이더가 default이나, 이 외의 여러개의 shader를 링크할 수 있어야 함.
        // reference 및 sharedpointer 형태를 사용하여(ShaderPtr) 다른 프로그램을 만드는데 재사용이 가능하게끔 함.
        static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);
        ~Program();
        uint32_t Get() const { return m_program; }    
    private:
        Program() {}
        // 초기화를 하는데 쓰일 link 함수.
        bool Link(const std::vector<ShaderPtr>& shaders);
        uint32_t m_program { 0 };
};


#endif