#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"



class Model
{
public:
    // model data
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    const aiScene* scene;

    Model() {}

    Model(string const& path, bool invertUV, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path, invertUV);
    }

    void Draw(Shader& shader);

    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);


private:

    void loadModel(string const& path, bool invertUV);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
};


