#pragma once
#include "CollisionType.h"
#include "Object3D.h"

class ICollider
{
protected:
	Object3D* object3d = nullptr;
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
public:
	ICollider() = default;
	virtual ~ICollider() = default;

	virtual void Update() = 0;

	inline void SetObject3D(Object3D* object) { object3d = object; }
	inline Object3D* GetObject3D() { return object3d; }
	inline CollisionShapeType GetShapeType() { return shapeType; }
};

