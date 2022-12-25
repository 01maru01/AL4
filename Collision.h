#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	static bool CheclSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
		Vector3D* inter = nullptr);
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		Vector3D* inter = nullptr);
};

