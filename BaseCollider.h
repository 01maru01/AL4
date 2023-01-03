#pragma once
#include "CollisionType.h"
#include "CollisionInfo.h"
#include "Object3D.h"

class BaseCollider
{
protected:
	Object3D* object3d = nullptr;
	CollisionShapeType shapeType = SHAPE_UNKNOWN;

	unsigned short attribute = 0b1111111111111111;
public:
	friend class CollisionManager;

	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	virtual void Update() = 0;

	inline void SetObject3D(Object3D* object) { object3d = object; }
	inline Object3D* GetObject3D() { return object3d; }
	inline CollisionShapeType GetShapeType() { return shapeType; }
	inline void OnCollision(const CollisionInfo& info) { object3d->OnCollision(info); }

	inline void SetAttribute(unsigned short attribute) { this->attribute = attribute; }
	inline void AddAttribute(unsigned short attribute) { this->attribute |= attribute; }
	inline void RemoveAttribute(unsigned short attribute) { this->attribute &= !attribute; }
};

