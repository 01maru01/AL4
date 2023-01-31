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
	//target += rightVec * (float)(input->GetKey(DIK_RIGHT) - input->GetKey(DIK_LEFT));
	//target += downVec * (float)(input->GetKey(DIK_DOWN) - input->GetKey(DIK_UP));
	//target += -frontVec * (float)(input->GetKey(DIK_Z) - input->GetKey(DIK_X));

	frontVec = target - eye;
	frontVec.normalize();
	rightVec = Vector3D(0, 1, 0).cross(frontVec);
	downVec = rightVec.cross(frontVec);
	
	if (rotAngle.x >= MyMath::PIx2) rotAngle.x -= MyMath::PIx2;
	if (rotAngle.x < 0) rotAngle.x += MyMath::PIx2;
	if (rotAngle.y >= MyMath::PIx2) rotAngle.y -= MyMath::PIx2;
	if (rotAngle.y < 0) rotAngle.y += MyMath::PIx2;

	Vector2D angle = rotAngle;
	angle += cursorSpd;

	rightVec.normalize();
	downVec.normalize();
#pragma region ビルボード
	billboard.Identity();
	billboard.m[0][0] = rightVec.x;
	billboard.m[0][1] = rightVec.y;
	billboard.m[0][2] = rightVec.z;
	billboard.m[1][0] = -downVec.x;
	billboard.m[1][1] = -downVec.y;
	billboard.m[1][2] = -downVec.z;
	billboard.m[2][0] = frontVec.x;
	billboard.m[2][1] = frontVec.y;
	billboard.m[2][2] = frontVec.z;

	billboardY.Identity();
	billboardY.m[0][0] = rightVec.x;
	billboardY.m[0][1] = rightVec.y;
	billboardY.m[0][2] = rightVec.z;
	Vector3D billYvecZ = rightVec.cross(up);
	billboardY.m[2][0] = billYvecZ.x;
	billboardY.m[2][1] = billYvecZ.y;
	billboardY.m[2][2] = billYvecZ.z;
#pragma endregion

	up.y = cosf(angle.y);
	eye.x = target.x - disEyeTarget * cosf(angle.y) * sinf(angle.x);
	eye.y = target.y + disEyeTarget * sinf(angle.y);
	eye.z = target.z - disEyeTarget * cosf(angle.y) * cosf(angle.x);
	MatUpdate();
}
