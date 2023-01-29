#pragma once
#include "MyMath.h"

class DirLight
{
public:
	struct ConstBufferData
	{
		Vector3D lightv;
		Vector3D lightcolor;
		unsigned int active;
	};

private:
	Vector3D lightdir = { 1,0,0 };
	Vector3D lightcolor = { 1,1,1 };
	bool active = false;

public:
	void SetLightDir(const Vector3D& lightdir_) { lightdir = lightdir_; }
	const Vector3D& GetLightDir() { return lightdir; }
	void SetLightColor(const Vector3D& lightcolor_) { lightcolor = lightcolor_; }
	const Vector3D& GetLightColor() { return lightcolor; }
	void SetActive(bool active_) { active = active_; }
	bool IsActive() { return active; }
};

