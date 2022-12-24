#pragma once
#include "CollisionType.h"
#include "CollisionInfo.h"
#include "Object3D.h"

class BaseCollider
{
protected:
	Object3D* object3d = nullptr;
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	virtual void Update() = 0;

	inline void SetObject3D(Object3D* object) { object3d = object; }
	inline Object3D* GetObject3D() { return object3d; }
	inline CollisionShapeType GetShapeType() { return shapeType; }
	inline void OnCollision(const CollisionInfo& info) { object3d->OnCollision(info); }
};

