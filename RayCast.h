#pragma once
#include "BaseCollider.h"

class Object3D;

struct RayCast
{
	Object3D* object = nullptr;
	BaseCollider* collider = nullptr;
	Vector3D inter;
	float distance = 0.0f;
};

