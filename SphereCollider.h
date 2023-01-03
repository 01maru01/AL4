#pragma once
#include "BaseCollider.h"
#include "MyMath.h"
#include "CollisionPrimitive.h"

class SphereCollider :public BaseCollider, public Sphere
{
private:
	Vector3D offset;
public:
	SphereCollider(Vector3D offset = Vector3D(), float radius = 1.0f);

	void Update() override;
	inline void SetRadius(float radius) { this->radius = radius; }
	inline float GetRadius() { return radius; }
	inline void SetOffset(const Vector3D& offset) { this->offset = offset; }
	inline const Vector3D& GetOffset() { return offset; }
};

