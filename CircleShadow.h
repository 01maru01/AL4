#pragma once
#include "MyMath.h"

class CircleShadow
{
public:
	struct ConstBuffData
	{
		Vector3D dir;
		float pad1;
		Vector3D casterPos;
		float distanceCasterLight;
		Vector3D atten;
		float pad2;
		Vector2D factorAngleCos;
		unsigned int active;
		float pad3;
	};

private:
	Vector3D dir = { 1,0,0 };
	Vector3D casterPos = { 0,0,0 };
	float distanceCasterLight = 100.0f;
	Vector3D atten = { 0.5f,0.6f,0.0f };
	Vector2D factorAngleCos = { 0.2f,0.5f };
	bool active = false;

public:
	void SetDir(const Vector3D& dir_) {
		dir = dir_;
		dir.normalize();
	}
	const Vector3D& GetDir() { return dir; }
	void SetCasterPos(const Vector3D& casterPos_) { casterPos = casterPos_; }
	const Vector3D& GetCasterPos() { return casterPos; }
	void SetDistanceCasterLight(float distanceCasterLight_) { distanceCasterLight = distanceCasterLight_; }
	float GetDistanceCasterLight() { return distanceCasterLight; }
	void SetAtten(const Vector3D& atten_) { atten = atten_; }
	const Vector3D& GetAtten() { return atten; }
	void SetFactorAngle(const Vector2D& factorAngle_) {
		factorAngleCos.x = cosf(MyMath::ConvertToRad(factorAngle_.x));
		factorAngleCos.y = cosf(MyMath::ConvertToRad(factorAngle_.y));
	}
	const Vector2D& GetFactorAngleCos() { return factorAngleCos; }
	void SetActive(bool active_) { active = active_; }
	bool IsActive() { return active; }
};

