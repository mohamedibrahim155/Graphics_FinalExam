#pragma once
#include <vector>
#include "PhysicsObject.h"




class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	float timer = 0;
	float fixedTimeStep = 0.01f;
	

	void AddPhysicsObjects(PhysicsObject* objs);
	void RemovePhysicsObject(PhysicsObject* objs);
	void RemoveAllPhysicsObject();
	void Update(float deltaTime);
	void UpdatePhysics(float deltatime);
	std::vector<PhysicsObject*> physicsObjects;
	std::vector<glm::vec3> collisionPoints;
	std::vector<glm::vec3> collisionNormals;




private:

};

