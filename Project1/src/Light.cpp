
#include "Light.h"

Light::Light()
{
	lightType = DIRECTION_LIGHT;
	this->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	//this->position = glm::vec3(0);
	this->ambient =  glm::vec4 (0.5f, 0.5f, 0.5f, 1.0f);        //default is low 
	this->diffuse =  glm::vec4 (0.5f, 0.5f, 0.5f, 1.0f);        //default is half 
	this->specular = glm::vec4 (0.5f, 0.5f, 0.5f, 1.0f);
	this->intensity = 0.5f;
	this->constant= 1.0f;
	this->linear = 1.0f;
	this->quadratic= 1.0f;
	this->color = glm::vec4(1,1 ,1,1);
	this->cutOffAngle = 12.5f;
	this->outerCutOffAngle = 15.0f;
}

void Light::Initialize(const LightType& type)
{
	this->lightType = type;

	this->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	//this->position = glm::vec3(0);
	this->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);        //default is low 
	this->diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);        //default is half 
	this->specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	this->intensity = 0.5f;
	this->constant = 1.0f;
	this->linear = 1.0f;
	this->quadratic = 1.0f;
	this->color = glm::vec4(1, 1, 1, 1);
	this->cutOffAngle = 12.5f;
	this->outerCutOffAngle = 15.0f;
}

void Light::Initialize(const LightType& type, const float& intensity)
{
	this->lightType = type;
	this->intensity = intensity;

	this->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	//this->position = glm::vec3(0);
	this->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);        //default is low 
	this->diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);        //default is half 
	this->specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	this->constant = 1.0f;
	this->linear = 1.0f;
	this->quadratic = 1.0f;
	this->color = glm::vec4(1, 1, 1, 1);
	this->cutOffAngle = 12.5f;
	this->outerCutOffAngle = 15.0f;
}

void Light::Initialize(Model* model, const LightType& type, const float& intensity)
{
	this->lightModel = model;
	this->lightType = type;
	this->intensity = intensity;

	this->direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	//this->position = glm::vec3(0);
	this->ambient = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);        //default is low 
	this->diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);        //default is half 
	this->specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	this->constant = 1.0f;
	this->linear = 1.0f;
	this->quadratic = 1.0f;
	this->color = glm::vec4(1, 1, 1, 1);
	this->cutOffAngle = 12.5f;
	this->outerCutOffAngle = 15.0f;
}

void Light::SetColor(const float x, const float y, const float z, const float w)
{
	this->color = glm::vec4(x, y, z, w);
}

void Light::SetColor(const glm::vec4& color)
{
	this->color = color;
}


