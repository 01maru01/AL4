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

struct Ray
{
	Vector3D start;
	Vector3D dir = { 1.0f,0.0f,0.0f };
};

class Triangle
{
public:
	Vector3D p0;
	Vector3D p1;
	Vector3D p2;
	Vector3D normal;

	void ComputeNormal();
};