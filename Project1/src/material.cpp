#include "material.h"



Material::Material()
{
    ambientType = AmbientType::Value;
    ambientColor = glm::vec4(1.0f);
    baseColor = glm::vec4(1.0f);
    ResetMaterial();
}

Material::Material( float shininess)
{
   
    //this->shininess = shininess;
   
   
}

Material::Material(const Material& material)
{
    SetBaseColor(glm::vec4(material.baseColor.x, material.baseColor.y, material.baseColor.z, material.baseColor.w));
    SetAmbientColor(glm::vec4(material.ambientColor.x, material.ambientColor.y, material.ambientColor.z, material.ambientColor.w));
    diffuseTexture = material.diffuseTexture;
    specularTexture = material.specularTexture;
    alphaTexture = material.alphaTexture;

    ambientType = material.ambientType;
    ambient= material.ambient;
    specular = material.specular;
    shininess = material.shininess;

    useMaskTexture = material.useMaskTexture;
    alphaCutoffThreshold = material.alphaCutoffThreshold;
}

Material::~Material()
{

    //need to do 
}
   

void Material::SetMaterialProperties(Shader& shader) 
{

    
    shader.setFloat("material.shininess", this->shininess);
    

}

void Material::ResetMaterial()
{
    baseColor = glm::vec4(1.0f);
    ambient = 1.0f;
    specular = 0.5f;
}

float Material::GetSpecular()
{
    return specular;
}

glm::vec4 Material::GetBaseColor()
{
    return baseColor;
}

glm::vec4 Material::GetAmbientColor()
{
    switch (ambientType)
    {
    case AmbientType::Color:
        return ambientColor;
    case AmbientType::Value:
        return baseColor * ambient;
    }
}

void Material::SetBaseColor(glm::vec4 color)
{
    baseColor = color;
}

void Material::SetAmbientColor(glm::vec4 color)
{
    ambientColor = color;
}

void Material::UpdateMaterial(Shader* shader)
{
}

void Material::ResetMaterial(Shader* shader)
{
}
