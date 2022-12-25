#pragma once
#include "MyMath.h"

struct Sphere {
	Vector3D center;
	float radius = 1.0f;
};

struct Plane {
	Vector3D normal = { 0.0f,1.0f,0.0f };
	float distance = 0.0f;
};