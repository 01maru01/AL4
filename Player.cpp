#include "Player.h"
#include "Input.h"

ICamera* Player::camera = nullptr;
const float Player::MAX_SPD = 0.1f;
const int Player::INVINCIBLE_TIME = 90;

void Player::SetCamera(ICamera* camera_)
{
	camera = camera_;
}

void Player::PlayerInitialize()
{
	Initialize("chr_sword", false);
	//mat.rotAngle.y = MyMath::PI;
	//mat.scale = { 2.0f,2.0f,2.0f };
}

void Player::Update()
{
	int front = Input::GetInstance()->GetKey(DIK_W) - Input::GetInstance()->GetKey(DIK_S);
	int side = Input::GetInstance()->GetKey(DIK_D) - Input::GetInstance()->GetKey(DIK_A);

	Vector3D moveVec;
	moveVec.x = front * camera->GetFrontVec().x + side * camera->GetRightVec().x;
	moveVec.z = front * camera->GetFrontVec().z + side * camera->GetRightVec().z;
	if (front != 0 && side != 0) {
		spd = MAX_SPD / 1.4142f;
	}
	else {
		spd = MAX_SPD;
	}
	moveVec *= spd;
	//mat.trans += moveVec;
	//camera->SetTarget({ mat.trans.x,mat.trans.y + 1.0f,mat.trans.z });
	camera->EyeMove(moveVec);
	camera->MatUpdate();

	if (Input::GetInstance()->GetKey(DIK_W) || Input::GetInstance()->GetKey(DIK_S) ||
		Input::GetInstance()->GetKey(DIK_A) || Input::GetInstance()->GetKey(DIK_D)) {
		//mat.rotAngle.y = atan2(moveVec.x, moveVec.z);
	}
}
