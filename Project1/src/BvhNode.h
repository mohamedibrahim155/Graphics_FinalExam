#include "PhysicsCollisionTypes.h"
#include"Triangle.h"
class BvhNode
{
public:
	BvhNode(const cAABB& aabb);
	 BvhNode(const cAABB& aabb, const std::vector<Triangle>& triangles,	 std::vector<int> triangleIndices, int nodeIndex, BvhNode* parentNode, Model* model);
	~BvhNode();


	BvhNode* rootNode = nullptr;
	BvhNode* leftChild;
	BvhNode* rightChild;
	std::vector<int> trianglesIndex;
	int nodeIndex = 0;
	cAABB aabb;
	
	cAABB GetModelAABB();
	void RecursiveSplit(const std::vector<Triangle>& triangleList);
	cAABB UpdateAABB();

private:
	int maxNumOfTriangles = 3;
	int maxtriCount = 3;
	int AABBDepth = 8;
	Model* model;
};

