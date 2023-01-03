#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

ICamera* Player::camera = nullptr;
const float Player::MAX_SPD = 0.1f;
const int Player::INVINCIBLE_TIME = 90;

void Player::SetCamera(ICamera* camera_)
{
	camera = camera_;
}

void Player::PlayerInitialize(Model* model)
{
	Initialize();
	SetModel(model);
	//mat.rotAngle.y = MyMath::PI;
	//mat.scale = { 2.0f,2.0f,2.0f 
	float radius = 0.5f;
	SetCollider(new SphereCollider(Vector3D(0, radius, 0), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
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

	if (!onGround) {
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		fallVec.y = max(fallVec.y + fallAcc, fallVYMin);
		mat.trans += fallVec;
	}
	else if (Input::GetInstance()->GetTrigger(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallVec = { 0,jumpVYFist,0 };
	}

	moveVec *= spd;
	mat.trans += moveVec;
	camera->SetTarget({ mat.trans.x,mat.trans.y + 1.0f,mat.trans.z });
	camera->EyeMove(moveVec);
	camera->MatUpdate();

	if (Input::GetInstance()->GetKey(DIK_W) || Input::GetInstance()->GetKey(DIK_S) ||
		Input::GetInstance()->GetKey(DIK_A) || Input::GetInstance()->GetKey(DIK_D)) {
		mat.rotAngle.y = atan2(moveVec.x, moveVec.z);
	}

	ColliderUpdate();
}

void Player::CollisionUpdate()
{
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.y += sphereCollider->GetRadius();
	ray.dir = { 0,-1.0,0 };
	RayCast raycastHit;

	if (onGround) {
		const float adsDis = 0.2f;

		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.0f + adsDis)) {
			onGround = true;
			mat.trans.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			ColliderUpdate();
			MatUpdate();
		}
		else {
			onGround = false;
			fallVec = {};
		}
	}
	else if (fallVec.y <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.0f)) {
			onGround = true;
			mat.trans.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			ColliderUpdate();
			MatUpdate();
		}
	}
}

void Player::OnCollision(const CollisionInfo& info)
{
	//mat.trans.x -= camera->GetFrontVec().x;
	//mat.trans.z -= camera->GetFrontVec().z;
	MatUpdate();
}
