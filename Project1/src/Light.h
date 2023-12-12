#pragma once
#include<glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include <vector>

enum LightType
{
    DIRECTION_LIGHT =0,
    POINT_LIGHT=1,
    SPOT_LIGHT =2
};

class Light
{
public:
    Light();
    ~Light() { };

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec3 direction;
    glm::vec4 color;
    LightType lightType;

    Model* lightModel;
   
    //for pointLight
    float constant;
    float linear;
    float quadratic;
    float intensity = 0.5f;

    //spot Lights
    float cutOffAngle;
    float outerCutOffAngle;

    GLint cuttOffAngle_UL;
    GLint outerCutOffAngle_UL;
    GLint quadratic_UL;
    GLint linear_UL;
    GLint constant_UL;
    GLint lightType_UL;
    GLint direction_UL;
    GLint specular_UL;
    GLint diffuse_UL;
    GLint ambient_UL;
    GLint position_UL;
    GLint color_UL;


private:
    Material material;
};







