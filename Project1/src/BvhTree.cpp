#include "BvhTree.h"
#include "PhysicsObject.h"

BvhTree::BvhTree(PhysicsObject* physicsObject)
{
	this->physicsObject = physicsObject;


    root = new BvhNode(physicsObject->aabb, physicsObject->listoftriangles, {}, 0, nullptr, physicsObject->model);
   // root = BuildBVH(physicsObject->GetModelAABB(), physicsObject->listoftriangles);
}

BvhTree::~BvhTree()
{
    delete root;
}

BvhNode* BvhTree::BuildBVH(const cAABB& aabb, std::vector<Triangle>& triangles, int iteration)
{
    if (iterationCount>4)
    {
        return nullptr;
    }
    BvhNode* rootNode = new BvhNode(aabb);


    if (triangles.size()==0)
    {
        return nullptr;
    }

    int maxExtentAxis = FindMaxExtentAxis(rootNode->aabb);

    auto compareFunction = [maxExtentAxis](const Triangle& a, const Triangle& b) -> bool {
        return a.center[maxExtentAxis] < b.center[maxExtentAxis];
        };
    std::sort(triangles.begin(), triangles.end(), compareFunction);

    size_t mid = triangles.size() / 2;

    std::vector<Triangle> leftTriangles(triangles.begin(), triangles.begin() + mid);
    std::vector<Triangle> rightTriangles(triangles.begin() + mid, triangles.end());
  
   


    std::pair<cAABB, cAABB> leftRightAABBs = SplitAABBAlongMaxExtent(aabb);
    cAABB leftAABB = leftRightAABBs.first;
    cAABB rightAABB = leftRightAABBs.second;

    rootNode->leftChild = BuildBVH(leftAABB,leftTriangles);
    rootNode->rightChild = BuildBVH(rightAABB,rightTriangles);
    iterationCount = iterationCount + 1;
    return rootNode;
}

void BvhTree::RecursiveSplit( const cAABB& aabb, int currentIteration, int maxIteration)
{

    if (currentIteration >= maxIteration) {
        // Maximum iterations reached, stop recursion
        return;
    }
    std::pair<cAABB, cAABB> leftRightAABBs = SplitAABBAlongMaxExtent(aabb);
    cAABB leftAABB = leftRightAABBs.first;
    cAABB rightAABB = leftRightAABBs.second;
  

    // RecursiveSplit(rootObject,leftRightAABBs.first, currentIteration + 1, maxIteration);
   //  RecursiveSplit(rootObject,leftRightAABBs.second, currentIteration + 1, maxIteration);

}

bool BvhTree::CompareTriangles(const Triangle& a, const Triangle& b, int maxExtentAxis)
{
    return a.center[maxExtentAxis] < b.center[maxExtentAxis];
}

void BvhTree::BuildNode()
{
   
}

