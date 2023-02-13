#include "MyDebugCamera.h"

MyDebugCamera::MyDebugCamera()
{
	input = Input::GetInstance();
}

MyDebugCamera::~MyDebugCamera()
{
}

void MyDebugCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	eye = eye_;
	target = target_;
	up = up_;

	MatUpdate();

	frontVec = target - eye;
	disEyeTarget = frontVec.length();
}

void MyDebugCamera::Update()
{
	moveCursor = input->GetCursor() - input->GetPrevCursor();
	float cursorDisPrev = moveCursor.length();
	moveCursor.normalize();

	if (input->Click(Input::LeftClick) && input->GetKey(DIK_LSHIFT)) {
		moveCursor /= 1000;
		moveCursor *= cursorDisPrev;
		if (up.y < 0) {
			moveCursor.x = -moveCursor.x;
		}
		cursorSpd += moveCursor;
	}
	disEyeTarget += -input->Wheel() * (disEyeTarget * 0.001f);
	if (disEyeTarget < 10) {
		disEyeTarget = 10;
	}
	target += rightVec * (float)(input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT));
	target += downVec * (float)(input->GetKey(DIK_DOWN) - input->GetKey(DIK_UP));
	target += -frontVec * (float)(input->GetKey(DIK_Z) - input->GetKey(DIK_X));
	
	if (rotAngle.x >= MyMath::PIx2) rotAngle.x -= MyMath::PIx2;
	if (rotAngle.x < 0) rotAngle.x += MyMath::PIx2;
	if (rotAngle.y >= MyMath::PIx2) rotAngle.y -= MyMath::PIx2;
	if (rotAngle.y < 0) rotAngle.y += MyMath::PIx2;

	Vector2D angle = rotAngle;
	angle += cursorSpd;

	//	カメラ方向ベクトル計算
	CalcCameraDirVec();

	//	ビルボード計算
	CalcBillboard();

	up.y = cosf(angle.y);
	eye.x = target.x - disEyeTarget * cosf(angle.y) * sinf(angle.x);
	eye.y = target.y + disEyeTarget * sinf(angle.y);
	eye.z = target.z - disEyeTarget * cosf(angle.y) * cosf(angle.x);
	MatUpdate();
}
