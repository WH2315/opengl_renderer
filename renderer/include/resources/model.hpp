#pragma once

#include "resources/mesh.hpp"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace wen {

class Model {
public:
    Model(const std::string& filename);
    ~Model();

    void draw(const std::shared_ptr<ShaderProgram>& program, uint32_t count = 1);

private:
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<ModelTexture> loadTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name);
    uint32_t createTexture(const std::string& name);

private:
    std::string directory_;
    std::map<std::string, std::shared_ptr<Mesh>> meshes_;
    std::vector<ModelTexture> textures_loaded_;
};

} // namespace wen