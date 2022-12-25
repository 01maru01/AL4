#pragma once
#include "BaseCollider.h"
#include "MyMath.h"
#include "CollisionPrimitive.h"

class SphereCollider :public BaseCollider, public Sphere
{
private:
	Vector3D offset;
public:
	SphereCollider(Vector3D offset = Vector3D(), float radius = 1.0f) :offset(offset) { shapeType = COLLISIONSHAPE_SPHERE; }

	void Update() override;
	inline void SetRadius(float radius) { this->radius = radius; }
};

