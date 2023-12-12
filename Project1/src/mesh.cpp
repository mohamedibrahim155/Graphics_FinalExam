#include "mesh.h"
#include <string>
#include "Renderer.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures)
{
    
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* meshMaterial)
{
    this->vertices = vertices;
    this->indices = indices;
    this->meshMaterial = meshMaterial;

    setupMesh();
}

void Mesh::meshDraw(Shader& shader)
{

    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int alphaNr = 0;
    //unsigned int normalNr = 1;
    //unsigned int emissiveNr = 1;
   // unsigned int heightNr = 1;
   

    for (unsigned int i = 0; i < textures.size(); i++)
    {
       GLCALL(glActiveTexture(GL_TEXTURE0 + i));
        std::string number;
        std::string name = textures[i]->type;
        if (name == "material.diffuse")
        {
            number = std::to_string(diffuseNr++);
            name = "diffuse";
        }
        else if (name == "material.specular")
        {
            number = std::to_string(specularNr++);
            name = "specular";
        }
        else if (name == "material.alphaMask")
        {
            number = std::to_string(alphaNr++);
            name = "alphaMask";
        }


        shader.setFloat("material.shininess", 128);

        if (isTransparancy)
        {
           // GLCALL(shader.setFloat("material.alpha", 1.0f));
            GLCALL(shader.setBool("isMasking", true));
        }
        else
        {
          //  GLCALL(shader.setFloat("material.alpha", 1.0f));
           shader.setBool("isMasking", false);
        }


        if (isCutOut)
        {
            GLCALL(shader.setBool("isCutout", true));
        }
        else
        {
            GLCALL(shader.setBool("isCutout", false));

        }

        if (isTextureScrolling)
        {
            GLCALL(shader.setBool("isScrollingTexture", true));

        }
        else
        {
            GLCALL(shader.setBool("isScrollingTexture", false));

        }

     

        shader.setInt((name), i);
         //std::cout << shader.FindUniformLocations((name + number).c_str()) << std::endl;
      //  GLCALL(glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i));

        GLCALL(glBindTexture(GL_TEXTURE_2D, textures[i]->id));
    }
    GLCALL(glActiveTexture(GL_TEXTURE0));


   VAO->Bind();
   IBO->Bind();

   if (isWireFrame)
   {
       GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
   }
   else
   {
       GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
   }
   
   GLCALL( glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0));
   GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
   VAO->Unbind();


  /* if (isTransparancy)
   {
       glDisable(GL_BLEND);
   }
  */

}

void Mesh::MeshDraw(Shader* shader)
{
    shader->Bind();  

    if (shader->shaderType == ShaderType::OPAQUE)
    {


        shader->setVec4("material.baseColor", meshMaterial->GetBaseColor().x, meshMaterial->GetBaseColor().y, meshMaterial->GetBaseColor().z, meshMaterial->GetBaseColor().w);
        shader->setVec4("material.ambientColor", meshMaterial->GetAmbientColor().x, meshMaterial->GetAmbientColor().y, meshMaterial->GetBaseColor().z, meshMaterial->GetAmbientColor().w);

        shader->setFloat("material.specularValue", meshMaterial->GetSpecular());
        shader->setFloat("material.shininess", meshMaterial->shininess);

        if (meshMaterial->diffuseTexture != nullptr)
        {

            GLCALL(glActiveTexture(GL_TEXTURE0 + 0));
            shader->setInt("diffuse_Texture", 0);
            meshMaterial->diffuseTexture->Bind();

        }
        if (meshMaterial->specularTexture != nullptr)
        {

            GLCALL(glActiveTexture(GL_TEXTURE0 + 1));
            shader->setInt("specular_Texture", 1);
            meshMaterial->specularTexture->Bind();

        }

        if (meshMaterial->alphaTexture != nullptr)
        {
            GLCALL(glActiveTexture(GL_TEXTURE0 + 2));
            shader->setInt("opacity_Texture", 2);
            meshMaterial->alphaTexture->Bind();
        }
    }
    else if (shader->shaderType == ShaderType::SOLID)
    {
        shader->setVec3("objectColor", glm::vec3(1, 1, 1));
    }
    VAO->Bind();
    IBO->Bind();

    if (isWireFrame)
    {
        GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    else
    {
        GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }


    GLCALL(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0));
    meshMaterial->diffuseTexture->Unbind();
    meshMaterial->specularTexture->Unbind();
    meshMaterial->alphaTexture->Unbind();
    VAO->Unbind();


}

void Mesh::SetTransparency(const bool& isTransparent)
{
    this->isTransparancy = isTransparent;
}

void Mesh::SetCutOff(const bool& isCutOut)
{

    this->isCutOut = isCutOut;

}

void Mesh::TextureScrolling(const bool& isScroll)
{

    this->isTextureScrolling = isScroll;

}

void Mesh::setupMesh()
{
    CalculateTriangles();
    VAO = new VertexArray();
    void* pVertices = static_cast<void*>(&vertices[0]);
    unsigned int size = vertices.size() * sizeof(Vertex);
    VBO = new VertexBuffer(pVertices, size);
    layout = new VertexBufferLayout();

    layout->Push<float>(3);
    layout->Push<float>(3);
    layout->Push<float>(2);
    layout->Push<float>(4);

    VAO->AddBuffer(*VBO, *layout);
    IBO = new IndexBufferObject(&indices[0], indices.size());

}

void Mesh::CalculateTriangles()
{
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Triangles tempTri;

        tempTri.v1 = vertices[indices[i]].Position;
        tempTri.v2 = vertices[indices[i + 1]].Position;
        tempTri.v3 = vertices[indices[i + 2]].Position;

        tempTri.normal = (vertices[indices[i]].Normal +
            vertices[indices[i + 1]].Normal +
            vertices[indices[i + 2]].Normal) / 3.0f;
        triangle.push_back(tempTri);
    }
}
