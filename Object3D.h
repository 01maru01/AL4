#pragma once
#include "CollisionInfo.h"

class ICollider;

class Object3D
{
protected:
	ICollider* collider = nullptr;
public:
	Object3D() = default;
	virtual ~Object3D();
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	virtual void OnCollision(const CollisionInfo& info) {};
};

