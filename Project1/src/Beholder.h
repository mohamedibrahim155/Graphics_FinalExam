#pragma once

#include "GraphicsRender.h"


class Beholder
{
public:

	Beholder();

	Beholder(GraphicsRender* render, Shader* shader);

	~Beholder();

	void LoadBeholderModel();

private:

	Model* model = nullptr;
	Model* SpotLight1 = nullptr;
	Model* SpotLight2 = nullptr;
	Model* SpotLight3 = nullptr;

	GraphicsRender* render;
	Shader* shader;

};

