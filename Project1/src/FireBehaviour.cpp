#include "FireBehaviour.h"

FireScale::FireScale()
{
}

FireScale::FireScale(GraphicsRender& renderer, Shader* shader, Texture* fireTexture)
{
	this->renderer = &renderer;
	this->shader = shader;
	this->fireTexture = fireTexture;
	this->model = nullptr;
	
}



FireScale::~FireScale()
{
}

void FireScale::LoadModel(Model* copyModel, const glm::vec3& spawnPoistion)
{
	model = new Model(*copyModel);
	glm::vec3 offset(0, -0.5f, 0);
	model->transform.SetPosition(spawnPoistion + offset);
	model->transform.SetRotation(glm::vec3(90,0,0));
	model->transform.SetScale(glm::vec3(1));


	renderer->AddModelsAndShader(model, shader);
}

void FireScale::Update(float deltaTime)
{
	float flicker = std::abs(CalculateFlickering(deltaTime) * 10.0f);

	if (model!=nullptr)
	{
		model->transform.SetScale(glm::vec3(flicker, flicker, flicker));
	}

}

float FireScale::CalculateFlickering(float deltaTime)
{
	totalTime += deltaTime;

	float flickerValue = flicker * sin(freqeuncy * totalTime);

	return flickerValue;
}
