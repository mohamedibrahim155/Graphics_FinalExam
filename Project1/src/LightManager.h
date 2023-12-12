#pragma once

#include "Light.h"
class LightManager
{
public:
    LightManager() { }
    ~LightManager() { }
    void SetUniformLocations(Light light, Shader& modelShader);
    void SetUniforms(GLuint shaderID);
    void AddNewLight(Light &light);
    // This is called every frame
    void UpdateUniformValues(GLuint shaderID);
    void UpdateUniformValuesToShader(Shader* shader);
    static const unsigned int MAX_LIGHT = 11;
    Light LightToIlluminate[MAX_LIGHT];
    std::vector<Light> lightList;
    
private:
};



