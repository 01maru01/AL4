#pragma once
#include "ICamera.h"

class GameCamera :public ICamera
{
private:
	Input* input = nullptr;

	bool rockOn = false;

	float disEyeTarget;

	Vector2D cursorMoveVec;
	Vector2D cursorSpd;

	Vector2D rotAngle;
	Vector2D prevRotAngle;
	Vector2D endRotAngle;
public:
	GameCamera();
	~GameCamera() override;

	void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) override;
	void Update() override;
};

