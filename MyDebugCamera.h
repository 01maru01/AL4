#pragma once
#include "MyMath.h"
#include "Input.h"
class MyDebugCamera
{
public:
	Matrix mat;
	Vector3D eye;		//	視点座標
	Vector3D target;	//	注視点座標
	Vector3D up;		//	上方向ベクトル
	Matrix billboard;
private:
	Input* input = nullptr;

	float disEyeTarget;
	Vector3D frontVec;
	Vector2D cursor;
	Vector2D prevCursor;
	Vector2D moveCursor;
	Vector2D cursorSpd;

	Vector2D rotAngle;
	Vector2D prevRotAngle;
	Vector2D endRotAngle;

	Vector3D rightVec;
	Vector3D downVec;
public:
	MyDebugCamera(Vector3D _eye, Vector3D _target, Vector3D _up);
	void Update();
	void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
	void MatUpdate();
	Vector3D Transfrom() { return eye; }
	Vector3D FrontVec() { return frontVec; }
	void Move(float camerapos);
};

