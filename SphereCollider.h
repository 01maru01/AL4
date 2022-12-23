#pragma once
#include "ICollider.h"
#include "MyMath.h"

class SphereCollider :public ICollider
{
private:
	Vector3D offset;

	Vector3D center;
	float radius;
public:
	SphereCollider(Vector3D offset = Vector3D(), float radius = 1.0f) :offset(offset), radius(radius) { shapeType = COLLISIONSHAPE_SPHERE; }

	void Update() override;
	inline void SetRadius(float radius) { this->radius = radius; }
};
