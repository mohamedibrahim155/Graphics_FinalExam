#pragma once
#ifndef MODEL_H
#define MODEL_H



#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "mesh.h"
#include "Shader.h"
#include "Transform.h"
#include"material.h"




class Model
{
public:


    std::vector<Texture*> textures_loaded;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string directory;
    std::string modelPath;
    Texture* alphaMask;

    int offset;
    std::string id; //if needed 
    float size;

    bool gammaCorrection;
    bool isTransparant = false;
    bool isCutOut = false;
    Transform transform;
    bool isWireFrame;
    bool isVisible =true;
    bool isLoadTexture;
    Model();
    Model(const Model& copyModel);
    Model( std::string const& path, bool isLoadTexture =true, bool isTextureFlip=true, bool isTransparancy= false, bool isCutOut = false);
    void loadModel(std::string const& path , bool isLoadTexture =true);
    void Draw(Shader& shader);
    

private:
    
    void processNode(aiNode* node, const aiScene* scene);   
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    Texture* LoadDefaultTexture(aiTextureType type, std::string typeName);
    Texture* LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
   
    bool isTextureFlipped = true;

    const std::string alphaTextureDefaultPath = "Textures/DefaultTextures/Opacity_Default.png";
};




#endif