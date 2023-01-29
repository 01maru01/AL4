#pragma once
#include "MyMath.h"

class SpotLight
{
public:
	struct ConstBuffData
	{
		Vector3D lightv;
		float pad1;
		Vector3D lightpos;
		float pad2;
		Vector3D lightcolor;
		float pad3;
		Vector3D lightatten;
		float pad4;
		Vector2D lightfactoranglecos;
		unsigned int active;
		float pad5;
	};

private:
	Vector3D lightdir = { 1,0,0 };
	Vector3D lightpos = { 0,0,0 };
	Vector3D lightcolor = { 1,1,1 };
	Vector3D lightAtten = { 1,1,1 };
	Vector2D lightFactorAngleCos = { 0.5f,0.2f };
	bool active = false;

public:
	void SetLightDir(const Vector3D& lightdir_) {
		lightdir = lightdir_;
		lightdir.normalize();
	}
	const Vector3D& GetLightDir() { return lightdir; }
	void SetLightPos(const Vector3D& lightPos) { lightpos = lightPos; }
	const Vector3D& GetLightPos() { return lightpos; }
	void SetLightColor(const Vector3D& color) { lightcolor = color; }
	const Vector3D& GetLightColor() { return lightcolor; }
	void SetLightAtten(const Vector3D& lightAtten_) { lightAtten = lightAtten_; }
	const Vector3D& GetLightAtten() { return lightAtten; }
	void SetLightFactorAngle(const Vector2D& lightFactorAngle) {
		lightFactorAngleCos.x = cosf(MyMath::ConvertToRad(lightFactorAngle.x));
		lightFactorAngleCos.y = cosf(MyMath::ConvertToRad(lightFactorAngle.y));
	}
	const Vector2D& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	void SetActive(bool active_) { active = active_; }
	bool IsActive() { return active; }
};

