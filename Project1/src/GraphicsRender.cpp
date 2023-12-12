#include "GraphicsRender.h"

GraphicsRender::GraphicsRender()
{
}

GraphicsRender::~GraphicsRender()
{
	ClearData();	
}

void GraphicsRender::AddModelsAndShader(Model* model, Shader* Shader)
{
	m_Models.push_back(model);
	m_Shaders.push_back(Shader);
}

void GraphicsRender::AddAlphaModelsAndShader(Model* model, Shader* Shader)
{
}

void GraphicsRender::AddModelsAndShader(Model& model, Shader& Shader)
{
	m_Models.push_back(&model);
	m_Shaders.push_back(&Shader);
}

void GraphicsRender::AddTransparentModels(Model* model, Shader* Shader)
{
	m_transparentModels.push_back(model);
	m_transparentShaders.push_back(Shader);

}

void GraphicsRender::SetDefaultCube(Model* cube)
{
	this->defaultBox = cube;
}


void GraphicsRender::AssignStencilShader(Shader* Shader)
{
	this->m_StencilShader = Shader;


}

void GraphicsRender::AssignLightShader(Shader* Shader)
{
	this->lightShader = Shader;
}

void GraphicsRender::AssignCamera(Camera* cam)
{

	this->cam = cam;
}

void GraphicsRender::SetLightShader(Shader* shader)
{
	this->lightShader = shader;
}

void GraphicsRender::SortObject()
{
	CompareDistances compareDistance(cam->Position);
	std::sort(m_transparentModels.begin(), m_transparentModels.end(), compareDistance);


}

std::vector<Model*> GraphicsRender::GetModelList()
{

	return m_Models;
}

void GraphicsRender::DrawAABB(const modelAABB& aabb)
{
	glm::vec3 targetExtents = 0.5f * (aabb.max - aabb.min);
	glm::vec3 center = 0.5f * (aabb.min + aabb.max);

	Model* debugCube = new Model(*defaultBox);
	debugCube->transform.SetPosition(center);
	debugCube->transform.SetRotation(glm::vec3(0));
	debugCube->transform.SetScale(targetExtents);
	//render.AddModelsAndShader(debugCube, defaultShader);
	debugCube->meshes[0]->isWireFrame = true;
	debugCube->Draw(*lightShader);
}








void GraphicsRender::Draw()
{
	/*std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < windows.size(); i++)
	{
		float distance = glm::length(camera.Position - windows[i]);
		sorted[distance] = windows[i];
	}*/

	glStencilMask(0x00);
	for (size_t i = 0; i < m_Models.size(); i++)
	{
		if (m_Models[i] == selectedModel)
		{

			continue;

		}
		m_Models[i]->Draw(*m_Shaders[i]);

	}

	if (selectedModel != nullptr)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glm::vec3 tempScale;


		tempScale = selectedModel->transform.scale;
		selectedModel->Draw(*m_Shaders[0]);
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
	
		
		glm::vec3 scale = selectedModel->transform.scale;
		
		selectedModel->transform.SetScale(scale + 0.01f);
		selectedModel->Draw(*m_StencilShader);
		selectedModel->transform.SetScale(tempScale);
		
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);


	SortObject();


	for (size_t i = 0; i < m_transparentModels.size(); i++)
	{
		
		m_transparentModels[i]->Draw(*m_transparentShaders[i]);

	}

}

void GraphicsRender::ClearData()
{
   //Deleteing model pointers
	if (m_Models.size() > 0)
	{
		for (Model* model : m_Models)
		{
			delete model;

		}
		m_Models.clear();
	}

	//Deleteing shader pointers
	if (m_Shaders.size() > 0)
	{
		for (Shader* shader : m_Shaders)
		{
			delete shader;

		}
		m_Shaders.clear();
	}
}
