#pragma once
#include"PhysicsEngine.h"
#include "GraphicsRender.h"
#include "InputManager.h"
class SpaceShip : public iInputListener
{
public:
	SpaceShip(GraphicsRender& render, Shader* shader, PhysicsEngine& engine, Camera& camera);
	SpaceShip();
	~SpaceShip();


	Model* model;
	PhysicsObject* SpaceShipPhysics;

	void LoadModel();

	void Update(float deltaTime);
	void DrawAABBCollision(PhysicsObject* physicsObject);
	void SpaceShipInputs(GLFWwindow* window, float deltaTime);

private:
	GraphicsRender* render;
	Shader* defaultshader;
	PhysicsEngine* engine;
	Camera* camera;

	glm::vec3 Direction = glm::vec3(0, 0, 0);

	float yoffset=2 ;
	float followDistance =2.5f;
	float speed = 5;
	float rotationAngle = 10;

	bool isDebugAAABDraw;

	void OnKeyPressed(const int& key) override;
	void OnKeyReleased(const int& key) override;
	void OnKeyHeld(const int& key) override;
};

