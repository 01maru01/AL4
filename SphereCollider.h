#pragma once
#include "BaseCollider.h"
#include "MyMath.h"

class SphereCollider :public BaseCollider
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

