#include "GameCamera.h"

GameCamera::GameCamera()
{
	input = Input::GetInstance();
}

GameCamera::~GameCamera()
{
}

void GameCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	eye = eye_;
	target = target_;
	up = up_;

	SetMatView();

	frontVec = target - eye;
	disEyeTarget = frontVec.length();
}

void GameCamera::Update()
{
	cursorMoveVec = input->GetCursor() - input->GetPrevCursor();
	float cursorDisPrev = cursorMoveVec.length();
	cursorMoveVec.normalize();

	//	ロックオンじゃないとき
	if (!rockOn) {
		cursorMoveVec /= 1000;
		cursorMoveVec *= cursorDisPrev;
		if (up.y < 0) {
			cursorMoveVec.x = -cursorMoveVec.x;
		}
		cursorSpd += cursorMoveVec;
	}

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

	if (cosf(angle.y) < 0.5f) {
		cursorSpd.y = MyMath::ConvertToRad(60.0f);
		angle.y = MyMath::ConvertToRad(60.0f);
	}
	up.y = cosf(angle.y);
	eye.x = target.x - disEyeTarget * cosf(angle.y) * sinf(angle.x);
	//	target.yはplayerの頭の高さ
	eye.y = target.y + disEyeTarget * sinf(angle.y);
	eye.z = target.z - disEyeTarget * cosf(angle.y) * cosf(angle.x);
	SetMatView();
}
