#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class MeshCollider :public BaseCollider
{
private:
	std::vector<Triangle> triangles;
	Matrix invMatWorld;
public:
	MeshCollider() { shapeType = COLLISIONSHAPE_MESH; }
	void ConstructTriangles(Model* model);
	void Update() override;
	bool CheckCollisionSphere(const Sphere& sphere, Vector3D* inter = nullptr);
	bool CheckCollisionRay(const Ray& ray, float* dis = nullptr, Vector3D* inter = nullptr);
};

