#pragma once
#include"GraphicsRender.h"

class FireScale
{
public:
	FireScale();
	FireScale(GraphicsRender& renderer, Shader* shader , Texture* fireTexture);
	~FireScale();

	void LoadModel(Model* copyModel , const glm::vec3& spawnPoistion);
	void Update(float deltaTime);

	float CalculateFlickering(float deltaTime);


private:

	Model* model = nullptr;
	GraphicsRender* renderer;
	Shader* shader;
	Texture* fireTexture;

	float totalTime = 0;
	float flicker = 0.1f;
	float freqeuncy = 1.5f;
};

