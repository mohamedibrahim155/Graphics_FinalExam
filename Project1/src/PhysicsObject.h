#pragma once
#include "model.h"
#include "PhysicsCollisionTypes.h"
#include <functional>
#include "BvhTree.h"

enum PhysicsType
{
	SPHERE,
	AABB,
	MESH_TRIANGLES,
	//MESH
};


enum ObjectMode
{
	STATIC,
	DYNAMIC
};


class PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(Model* model);
	~PhysicsObject();

	
	

	Model* model;
	PhysicsType physicsType;
	ObjectMode mode =STATIC;
	glm::vec3 position = glm::vec3(0.0f);
	bool isKinematic;
	bool collisionCallbool;
	void update(float deltatime);
	cAABB UpdateAABB();
	cSphere UpdateSphere();
	void CalculateTriangle();
	std::function<void(PhysicsObject*)> collisionCallback = nullptr;
	std::vector<Triangle> listoftriangles;
	 std::vector<cSphere*>  triangleSpheres;
	 std::vector<cAABB>  collisionAABBs;

	cAABB GetModelAABB();
	
	std::vector<Triangle> GetModelTriangleList();
	const std::vector<cAABB>& GetCollisionAABBs();

	//bool checkCollision( PhysicsObject& other);
	bool checkCollision(PhysicsObject* other, std::vector<glm::vec3>& collisionPoints, std::vector<glm::vec3>& collisionNormals);
	glm::vec3 velocity = glm::vec3(0);
	glm::vec3 acceleration;
	float gravityValue=0.1f;
	void Initialize(PhysicsType physicsType = PhysicsType::AABB , bool collision = false,ObjectMode mode = ObjectMode::STATIC);

	cAABB aabb;
	cSphere sphereShape;
	BvhTree* BvhAABBTree;
	bool isBvhActive = true;

	cAABB CalculateModelAABB();
	
	glm::vec3 GetPosition();

	void SetPosition(const glm::vec3& Position);
	const std::function<void(PhysicsObject*)>& GetCollisionCall();
	void DoCollisionCall(const std::function<void(PhysicsObject*)>& collisionCallback);
	float inverseMass = 1.0f;
	float bounciness = 0.8f;
	float mass = 1.0f;
	void SetMass(const float& massValue);



};



