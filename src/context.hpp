#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include "common.hpp"
#include "shader.hpp"
#include "program.hpp"


CLASS_PTR(Context)
class Context {
    public:
        static ContextUPtr Create();
        void Render();    
    private:
        Context() {}
        bool Init();

        ProgramUPtr m_program;
        uint32_t m_vertexArrayObject;
        uint32_t m_vertexBuffer;
        uint32_t m_indexBuffer;
};


#endif