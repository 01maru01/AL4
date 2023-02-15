#pragma once
#include "MyMath.h"
#include "Input.h"
#include "Window.h"

class ICamera
{
protected:
	//	ビュー行列
	MyMath::MatView view;
	//	射影変換行列
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);
public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) = 0;
	virtual void Update() = 0;
	void SetMatView();

	void EyeMove(Vector3D moveE) { view.eye += moveE; }

#pragma region Getter
	const Matrix& GetView() { return view.mat; }
	const Vector3D& GetEye() { return view.eye; }
	const Vector3D& GetTarget() { return view.target; }
	const Vector3D& GetUp() { return view.up; }
	//	カメラ方向ベクトル
	const Vector3D& GetFrontVec() { return view.frontVec; }
	const Vector3D& GetRightVec() { return view.rightVec; }
	const Vector3D& GetDownVec() { return view.downVec; }
	//	billboard
	const Matrix& GetBillboard() { return view.billboard; }
	const Matrix& GetBillboardY() { return view.billboardY; }
	//	射影変換行列
	const Matrix& GetProjection() { return matProjection; }
	const Matrix GetViewProj() {
		Matrix mat = view.mat;
		mat *= matProjection;
		return mat;
	}
#pragma endregion

#pragma region Setter
	void SetTarget(const Vector3D& target_) { view.target = target_; }
	void SetEye(const Vector3D& eye_) { view.eye = eye_; }
	void SetUp(const Vector3D& up_) { view.up = up_; }
	void SetProjection(float fovY, float nearZ, float farZ, int width_ = Window::window_width, int height_ = Window::window_height);
#pragma endregion
};

