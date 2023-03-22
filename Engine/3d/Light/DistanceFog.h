#pragma once
#include "MyMath.h"

class DistanceFog
{
public:
	struct ConstBuffData
	{
		Vector3D color;
		unsigned int active;
		float start;
		float end;
		float fogNear;
		float fogFar;
	};

private:
	Vector3D color = Vector3D(1.0f, 1.0f, 1.0f);
	float start = 0.1f;
	float end = 5.0f;
	float fogFar = 30.0f;
	float fogNear = 0.1f;
	bool active = false;
public:
	void SetColor(const Vector3D& color_) { color = color_; }
	const Vector3D& GetColor() { return color; }
	void SetStart(const float fogStart) { start = fogStart; }
	const float GetStart() { return start; }
	void SetEnd(const float fogEnd) { end = fogEnd; }
	const float GetEnd() { return end; }
	void SetFar(const float far_) { fogFar = far_; }
	const float GetFar() { return fogFar; }
	void SetNear(const float near_) { fogNear = near_; }
	const float GetNear() { return fogNear; }
	void SetActive(bool active_) { active = active_; }
	bool IsActive() { return active; }
};

