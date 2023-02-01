#pragma once
#include "BaseCollider.h"
#include "MyMath.h"
#include "CollisionPrimitive.h"

class PlaneCollider :public BaseCollider, public Plane
{
private:
	Vector3D offset;
public:
	PlaneCollider(Vector3D normal = { 0.0f,1.0f,0.0f }, float distance_ = 0.0f);

	void Update() override;
	inline void SetDistance(float distance_) { distance = distance_; }
	inline float GetDistance() { return distance; }
	inline void SetOffset(const Vector3D& offset_) { offset = offset_; }
	inline const Vector3D& GetOffset() { return offset; }
};