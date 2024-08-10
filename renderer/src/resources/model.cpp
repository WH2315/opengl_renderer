#include "resources/model.hpp"
#include "core/log.hpp"
#include "resources/texture_2d.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace internal {

static std::string BasePath(const std::string& filename) {
    if (filename.find_last_of("/\\") != std::string::npos) {
        return filename.substr(0, filename.find_last_of("/\\"));
    }
    return "";
}

} // namespace internal

namespace wen {

Model::Model(const std::string& filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        WEN_ERROR("Failed to load model: {0}", filename)
        throw std::runtime_error(importer.GetErrorString());
    }
    directory_ = internal::BasePath(filename);
    processNode(scene->mRootNode, scene);
}

void Model::draw(const std::shared_ptr<ShaderProgram>& program, uint32_t count) {
    for (const auto& mesh : meshes_) {
        mesh.second->draw(program, count);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.insert(std::make_pair(mesh->mName.C_Str(), processMesh(mesh, scene)));
    }
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<ModelTexture> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        if (mesh->HasNormals()) {
            vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        } else {
            vertex.normal = {0.0f, 0.0f, 0.0f};
        }
        if (mesh->mTextureCoords[0]) {
            vertex.tex_coords = {mesh->mTextureCoords[0][i].x, 1 - mesh->mTextureCoords[0][i].y};
        } else {
            vertex.tex_coords = {0.0f, 0.0f};
        }
        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    std::vector<ModelTexture> diffuse_maps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    std::vector<ModelTexture> specular_maps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    std::vector<ModelTexture> normal_maps = loadTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
    std::vector<ModelTexture> height_maps = loadTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<ModelTexture> Model::loadTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name) {
    std::vector<ModelTexture> textures;
    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(uint32_t j = 0; j < textures_loaded_.size(); j++) {
            if(std::strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded_[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            ModelTexture texture;
            texture.texture = createTexture(str.C_Str());
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded_.push_back(texture);
        }
    }
    return textures; 
}

uint32_t Model::createTexture(const std::string& name) {
    std::string filename = directory_ + '/' + name;

    auto texture = new Texture2D(filename, {
        .min_filter = TextureFilter::eLinearMipmapLinear,
        .mag_filter = TextureFilter::eLinear,
        .wrap_s = TextureWrap::eRepeat,
        .wrap_t = TextureWrap::eRepeat
    });

    return texture->getTexture();
}

Model::~Model() {
    meshes_.clear();
}

} // namespace wen