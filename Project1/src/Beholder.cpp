#include "Beholder.h"



Beholder::Beholder()
{
}

Beholder::Beholder(GraphicsRender* render, Shader* shader)
{
	this->render = render;
	this->shader = shader;
	this->model = nullptr;
}



Beholder::~Beholder()
{
}

void Beholder::LoadBeholderModel()
{
	model = new Model("Models/Exam_Models/3D_models/3D_models/beholder/export_ply/BeholderSketchfab_xyz_n_rgba_uv.ply");
	model->transform.SetPosition(glm::vec3(10, 4, 15));
	model->transform.SetScale(glm::vec3(1));
	render->AddModelsAndShader(model, shader);
	
}
