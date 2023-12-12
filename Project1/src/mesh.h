#pragma once
#ifndef MESH_H
#define MESH_H


#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"



#include <string>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Textures.h"
#include "Vertex.h"
#include "Triangle.h"
#include "material.h"
#define MAX_BONE_INFLUENCE 4




class Mesh
{
public:

   

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*>      textures;
    Material* meshMaterial;
    //unsigned int VAO;
    std::vector<Triangles> triangle;

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures);
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* meshMaterial);
    void meshDraw(Shader& shader);
    void MeshDraw(Shader* shader);
    void SetTransparency(const bool& isTransparent);
    void SetCutOff(const bool& isCutOut);
    void TextureScrolling(const bool& isScroll);

    VertexBuffer* VBO;
    IndexBufferObject* IBO;
    VertexArray* VAO;
    VertexBufferLayout* layout;
    bool isWireFrame = false;

private:
    
    //unsigned int VBO, EBO;

    void setupMesh();
    void CalculateTriangles();
    bool isTransparancy = false;
    bool isCutOut = false;
    bool isTextureScrolling = false;
};
#endif
