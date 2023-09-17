#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "Common.hpp"
#include "Buffer.hpp"
#include "VertexLayout.hpp"
#include "Texture.hpp"
#include "Program.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

CLASS_PTR(Material);
class Material {
    public:
        static MaterialUPtr Create() {
            return MaterialUPtr(new Material());
        }
        TexturePtr diffuse;
        TexturePtr specular;
        float shininess { 32.0f };

        void SetToProgram(const Program* program) const;

    private:
        Material() {}
};


CLASS_PTR(Mesh);
class Mesh {
    public:
        // vertex structure vector를 입력받고, primitiveType으로 그리려는 타입(삼각형, line, quad 등)으로 그림.
        static MeshUPtr Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType);
        static MeshUPtr MakeBox();

        void SetMaterial(MaterialPtr material) { m_material = material; }
        MaterialPtr GetMaterial() const { return m_material; }


        const VertexLayout* GetVertexLayout() const { return m_vertexLayout.get(); }
        BufferPtr GetVertexBuffer() const { return m_vertexBuffer; }
        BufferPtr GetIndexBuffer() const { return m_indexBuffer; }

        void Draw(const Program* program) const;

    private:
        Mesh() {}
        void Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType);

        uint32_t m_primitiveType { GL_TRIANGLES };
        // vbo, ibo는 다른 vao와 연결하여 재사용성이 있으나, vao는 해당 메쉬를 그리는 데만 사용되기에 각각 shared pointer, unique pointer를 쓴다.
        VertexLayoutUPtr m_vertexLayout;
        BufferPtr m_vertexBuffer;
        BufferPtr m_indexBuffer;
        MaterialPtr m_material;
};

#endif
