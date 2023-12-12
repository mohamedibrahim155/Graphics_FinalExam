#pragma once

#include<vector>
#include "BvhNode.h" 
#include <limits>
#include <algorithm>

class  PhysicsObject;
class BvhTree
{
public:
	BvhTree(PhysicsObject* physicsObject);
	~BvhTree();
	BvhNode* root = nullptr;
private:
	
	PhysicsObject* physicsObject = nullptr;
	std::vector<Triangle> transformedTriangles;
	BvhNode* BuildBVH(const cAABB& aabb ,  std::vector<Triangle>&  triangles, int iteration =4);
	int iterationCount=0;
	void RecursiveSplit( const cAABB& aabb, int currentIteration, int maxIteration);
	bool CompareTriangles(const Triangle& a, const Triangle& b, int maxExtentAxis);

	void BuildNode();

};

