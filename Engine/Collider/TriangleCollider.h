#pragma once
#include "BaseCollider.h"
#include "MyMath.h"
#include "CollisionPrimitive.h"

class TriangleCollider :public BaseCollider, public Triangle
{
private:
	Vector3D offset;
public:
	TriangleCollider(Vector3D p0_ = { 0.0f,1.0f,0.0f }, Vector3D p1_ = { 0.0f,1.0f,0.0f }, Vector3D p2_ = { 0.0f,1.0f,0.0f }, Vector3D normal_ = { 0.0f,1.0f,0.0f });

	void Update() override;
	inline void SetNormal(const Vector3D& normal_) { normal = normal_; }
	inline const Vector3D& GetNormal() { return normal; }
};