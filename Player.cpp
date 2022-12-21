#include "Player.h"
#include "Input.h"

void Player::PlayerInitialize()
{
	Initialize("chr_sword", false);
	mat.rotAngle.y = MyMath::PI;
}

void Player::Update()
{
	mat.trans.x += Input::GetInstance()->GetKey(DIK_D) - Input::GetInstance()->GetKey(DIK_A);
	mat.trans.z += Input::GetInstance()->GetKey(DIK_W) - Input::GetInstance()->GetKey(DIK_S);
}