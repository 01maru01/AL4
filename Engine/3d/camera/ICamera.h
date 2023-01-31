#pragma once
#include "MyMath.h"
#include "Input.h"
#include "Window.h"
class ICamera
{
protected:
	Matrix matView;
	Vector3D eye;		//	視点座標
	Vector3D target;	//	注視点座標
	Vector3D up;		//	上方向ベクトル
	Vector3D frontVec;
	Vector3D rightVec;
	Vector3D downVec;

	Matrix billboard;
	Matrix billboardY;
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);
public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) = 0;
	virtual void Update() = 0;
	void MatUpdate();

	//	Getter
	Vector3D GetEye() { return eye; }
	Vector3D GetTarget() { return target; }
	Vector3D GetUp() { return up; }
	Vector3D GetFrontVec() { return frontVec; }
	Vector3D GetRightVec() { return rightVec; }
	Vector3D GetDownVec() { return downVec; }
	Matrix GetView() { return matView; }
	Matrix GetProjection() { return matProjection; }
	Matrix GetBillboard() { return billboard; }
	Matrix GetBillboardY() { return billboardY; }
	Matrix GetViewProj() { 
		Matrix mat = matView;
		mat *= matProjection;
		return mat;
	}
	//	Setter
	void SetTarget(Vector3D t) { target = t; }
	void SetEye(Vector3D e) { eye = e; }
	void EyeMove(Vector3D moveE) { eye += moveE; }
};

