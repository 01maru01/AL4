#pragma once
#include "MyMath.h"
#include "Input.h"
#include "ICamera.h"
class MyDebugCamera :public ICamera
{
public:
	Matrix billboard;
private:
	Input* input = nullptr;

	float disEyeTarget = 0.0f;
	
	Vector2D cursor;
	Vector2D prevCursor;
	Vector2D moveCursor;
	Vector2D cursorSpd;

	Vector2D rotAngle;
	Vector2D prevRotAngle;
	Vector2D endRotAngle;
public:
	MyDebugCamera();

	void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) override;
	void Update() override;
};

