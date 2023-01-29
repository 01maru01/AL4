#pragma once
#include "Object3D.h"
#include "ICamera.h"

class Player :public Object3D
{
private:
	Vector2D frontVec;
	float spd;

	bool onGround = true;
	Vector3D fallVec;

	static ICamera* camera;
	static const float MAX_SPD;
	static const int INVINCIBLE_TIME;
public:
	static void SetCamera(ICamera* camera_);
	void PlayerInitialize(Model* model_);
	void Update();
	void CollisionUpdate();
	void OnCollision(const CollisionInfo& info) override;

	const Vector3D& GetPosition() { return mat.trans; }
};

