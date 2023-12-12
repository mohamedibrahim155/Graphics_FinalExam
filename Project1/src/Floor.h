#pragma once
#include "GraphicsRender.h"
class Floor
{
public:

	Floor();
	~Floor();

	void LoadModel(Model* model);
	void LoadModel(const std::string& string);

private:
	GraphicsRender* render;

	Model* floorModel;
};


