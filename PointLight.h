#pragma once
#include "MyMath.h"

class PointLight
{
public:
	struct ConstBuffData
	{
		Vector3D pos;
		float pad1;
		Vector3D color;
		float pad2;
		Vector3D lightatten;
		unsigned int active;
	};
private:
	Vector3D lightpos;
	Vector3D lightcolor = { 1.0f,1.0f,1.0f };
	Vector3D lightAtten = { 1.0f,1.0f,1.0f };
	bool active = false;
public:
	void SetLightPos(const Vector3D& lightPos) { lightpos = lightPos; }
	const Vector3D& GetLightPos() { return lightpos; }
	void SetLightColor(const Vector3D& color) { lightcolor = color; }
	const Vector3D& GetLightColor() { return lightcolor; }
	void SetLightAtten(const Vector3D& lightAtten_) { lightAtten = lightAtten_; }
	const Vector3D& GetLightAtten() { return lightAtten; }
	void SetActive(bool active_) { active = active_; }
	bool IsActive() { return active; }
};

