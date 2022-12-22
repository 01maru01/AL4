#pragma once
#include "MyMath.h"
#include "Input.h"
class ICamera
{
protected:
	Matrix mat;
	Vector3D eye;		//	視点座標
	Vector3D target;	//	注視点座標
	Vector3D up;		//	上方向ベクトル
	Vector3D frontVec;
	Vector3D rightVec;
	Vector3D downVec;

	void MatUpdate();
public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) = 0;
	virtual void Update() = 0;

	//	Getter
	Vector3D GetEye() { return eye; }
	Vector3D GetTarget() { return target; }
	Vector3D GetUp() { return up; }
	Vector3D GetFrontVec() { return frontVec; }
	Vector3D GetRightVec() { return rightVec; }
	Vector3D GetDownVec() { return downVec; }
	Matrix GetMatrix() { return mat; }
};

