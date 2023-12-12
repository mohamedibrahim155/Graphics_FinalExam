#pragma once
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexArray.h"
#include  "model.h"
#include "material.h"
#include "Light.h"
#include "Transform.h"
#include"LightManager.h"
#include "Random.h"
#include "PhysicsEngine.h"
#include "Skybox.h"
#include "LoadModelsExternal.h"
#include "GraphicsRender.h"
#include "CameraMovement.h"
#include "SpaceShip.h"
#include "InputManager.h"
struct ModelData
{


	std::string path;
	glm::vec3 position;
	glm::vec3 rotation;
	bool isCutoff;
	bool isTrans;
	std::string texturePath;	
	glm::vec3 scale;
};

class ApplicationRenderer
{
public:
	ApplicationRenderer();
	~ApplicationRenderer();

	static void SetViewPort(GLFWwindow* window, int width, int height);
	void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseCallBack(GLFWwindow* window, double xposIn, double yposIn);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	
	void WindowInitialize(int width, int height,  std::string windowName ="Window");

	std::vector<ModelData> modelData;
	std::vector<ModelData> loadModelDataFromFile(const std::string& filePath);
	
	void Start();
	void PreRender();
	void Render();
	void PostRender();
	void Clear();

	void ProcessInput(GLFWwindow* window);
	
	void DrawDebugModelAABB(const cAABB& aabb);
	void DrawDebugBvhNodeAABB(BvhNode* node);

	int recusiveCount = 0;


private:
	GLFWwindow* window;

	Camera camera;
	CameraMovement moveCam;

	GraphicsRender render;
	LightManager lightManager;

	Shader* defaultShader;
	Shader* lightShader;
	Shader* StencilShader;
	Shader* SkyboxShader;

	Skybox* skybox;


	Model* defaultBox;
	Model* CityModel;

	LoadModelsExternal m_loadModels;

	PhysicsEngine PhysicsEngine;
	PhysicsObject* cityPhysics;


	SpaceShip* spaceshipEntity;

	 int windowWidth;
	 int  WindowHeight;
	 int selectedModelCount = 0;

	 float lastX;
	 float lastY;
	 float deltaTime;
	 double lastTime;
	 float lastFrame;

	 float scrollTime;

	 bool firstMouse;
	 bool cameraMoveToTarget;
};

