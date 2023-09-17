#include "Model.hpp"

ModelUPtr Model::Load(const std::string& filename) {
    auto model = ModelUPtr(new Model());
    if (!model->LoadByAssimp(filename))
        return nullptr;
    return std::move(model);
}

// Assimp::Importer크ㄹ래스의 ReadFile 함수 이용.
// scene->mRootNode부터 재귀적으로 처리.
// Scene Tree상 이미지가 부모/자식 관계로 되어있기 때문.
bool Model::LoadByAssimp(const std::string& filename) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        SPDLOG_ERROR("failed to load model: {}", filename);
        return false;
    }

    auto dirname = filename.substr(0, filename.find_last_of("/"));
    // ramda closer. 함수 내부에 선언가능하다.
    // [capture, ...] (param_type parameters, ...) -> return_type {
    // ... closure body statements } 식의 형태.
    // parameter에 &가 붙음으로서, 외부의 값을 capture해서 외부의 값들도 사용 가능하게 할 수 있다.
    auto LoadTexture = [&](aiMaterial* material, aiTextureType type) -> TexturePtr {
        if (material->GetTextureCount(type) <= 0)
            return nullptr;
        aiString filepath;
        material->GetTexture(type, 0, &filepath);
        auto image = Image::Load(fmt::format("{}/{}", dirname, filepath.C_Str()));
        if (!image)
            return nullptr;
        return Texture::CreateFromImage(image.get());
    };

    for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
        auto material = scene->mMaterials[i];
        auto glMaterial = Material::Create();
        glMaterial->diffuse = LoadTexture(material, aiTextureType_DIFFUSE);
        glMaterial->specular = LoadTexture(material, aiTextureType_SPECULAR);
        m_materials.push_back(std::move(glMaterial));
    }

    ProcessNode(scene->mRootNode, scene);
    return true;
}

// 위에서 언급했듯, 재귀형태로 처리되어있음.
void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    // mesh가 먼저 있는지 확인하고, 있으면 해당 mesh를 처리.
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        auto meshIndex = node->mMeshes[i];
        auto mesh = scene->mMeshes[meshIndex];
        ProcessMesh(mesh, scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

// aiMesh, aiScene이라는 assimp 내부의 클래스를 입력으로 받아 mesh의 data를 로딩한다.
void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    SPDLOG_INFO("process mesh: {}, #vert: {}, #face: {}",
        mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

    // 벡터 형태의 vertex array, index array를 만든다.
    // 각각의 vertex, index정보를 집어넣어 만든다.
    std::vector<Vertex> vertices;
    vertices.resize(mesh->mNumVertices);
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        auto& v = vertices[i];
        v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        v.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    std::vector<uint32_t> indices;
    // triangle * 3을 하면 index의 개수이다.
    indices.resize(mesh->mNumFaces * 3);
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        indices[3 * i] = mesh->mFaces[i].mIndices[0];
        indices[3*i+1] = mesh->mFaces[i].mIndices[1];
        indices[3*i+2] = mesh->mFaces[i].mIndices[2];
    }

    auto glMesh = Mesh::Create(vertices, indices, GL_TRIANGLES);
    if (mesh->mMaterialIndex >= 0)
        glMesh->SetMaterial(m_materials[mesh->mMaterialIndex]);
    
    m_meshes.push_back(std::move(glMesh));
}

void Model::Draw(const Program* program) const {
    for (auto& mesh: m_meshes) {
        mesh->Draw(program);
    }
}
