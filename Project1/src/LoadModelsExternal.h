#pragma once

#include "Renderer.h"
#include "model.h"
#include "GraphicsRender.h"

class LoadModelsExternal
{
public:

	LoadModelsExternal();
	~LoadModelsExternal();


	void LoadAllModels(Shader& shader);

	GraphicsRender render;





};

