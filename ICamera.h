#pragma once
#include "MyMath.h"
#include "Input.h"
#include "Window.h"

class ICamera
{
protected:
	Vector3D eye;		//	視点座標
	Vector3D target;	//	注視点座標
	Vector3D up;		//	上方向ベクトル
	//	カメラの方向ベクトル
	Vector3D frontVec;
	Vector3D rightVec;
	Vector3D downVec;
	//	ビルボード
	Matrix billboard;
	Matrix billboardY;
	//	ビュー行列
	Matrix matView;
	//	射影変換行列
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);

	void CalcBillboard();		//	ビルボード計算
	void CalcCameraDirVec();	//	カメラ方向ベクトル計算
public:
	virtual ~ICamera() = default;
	virtual void Initialize(Vector3D eye_, Vector3D target_, Vector3D up_) = 0;
	virtual void Update() = 0;
	void MatUpdate();

	void EyeMove(Vector3D moveE) { eye += moveE; }

#pragma region Getter
	const Matrix& GetView() { return matView; }
	const Vector3D& GetEye() { return eye; }
	const Vector3D& GetTarget() { return target; }
	const Vector3D& GetUp() { return up; }
	//	カメラ方向ベクトル
	const Vector3D& GetFrontVec() { return frontVec; }
	const Vector3D& GetRightVec() { return rightVec; }
	const Vector3D& GetDownVec() { return downVec; }
	//	billboard
	const Matrix& GetBillboard() { return billboard; }
	const Matrix& GetBillboardY() { return billboardY; }
	//	射影変換行列
	const Matrix& GetProjection() { return matProjection; }
	const Matrix& GetViewProj() {
		Matrix mat = matView;
		mat *= matProjection;
		return mat;
	}
#pragma endregion

#pragma region Setter
	void SetTarget(const Vector3D& target_) { target = target_; }
	void SetEye(const Vector3D& eye_) { eye = eye_; }
	void SetUp(const Vector3D& up_) { up = up_; }
	void SetProjection(float fovY, float nearZ, float farZ, float width_ = Window::window_width, float height_ = Window::window_height);
#pragma endregion
};

