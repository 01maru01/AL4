#pragma once
#include "MyMath.h"
#include "Input.h"
#include "Window.h"

class ICamera
{
protected:
	Vector3D eye;		//	���_���W
	Vector3D target;	//	�����_���W
	Vector3D up;		//	������x�N�g��
	//	�J�����̕����x�N�g��
	Vector3D frontVec;
	Vector3D rightVec;
	Vector3D downVec;
	//	�r���{�[�h
	Matrix billboard;
	Matrix billboardY;
	//	�r���[�s��
	Matrix matView;
	//	�ˉe�ϊ��s��
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(48.0f), 0.1f, 1000.0f);

	void CalcBillboard();		//	�r���{�[�h�v�Z
	void CalcCameraDirVec();	//	�J���������x�N�g���v�Z
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
	//	�J���������x�N�g��
	const Vector3D& GetFrontVec() { return frontVec; }
	const Vector3D& GetRightVec() { return rightVec; }
	const Vector3D& GetDownVec() { return downVec; }
	//	billboard
	const Matrix& GetBillboard() { return billboard; }
	const Matrix& GetBillboardY() { return billboardY; }
	//	�ˉe�ϊ��s��
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

