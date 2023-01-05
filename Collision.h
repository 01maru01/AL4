#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	static void ClosestPtPoint2Triangle(const Vector3D& point, const Triangle& triangle,
		Vector3D* closest);
	//	Sphere
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
		Vector3D* inter = nullptr, Vector3D* reject = nullptr);
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		Vector3D* inter = nullptr);
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		Vector3D* inter = nullptr, Vector3D* reject = nullptr);
	//	Ray
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3D* inter = nullptr);
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, Vector3D* inter = nullptr);
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, Vector3D* inter = nullptr);
};

