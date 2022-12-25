#pragma once
#include "Object3D.h"
#include "ICamera.h"

class Player :public Object3D
{
private:
	Vector2D frontVec;
	float spd;

	static ICamera* camera;
	static const float MAX_SPD;
	static const int INVINCIBLE_TIME;
public:
	static void SetCamera(ICamera* camera_);
	void PlayerInitialize(Model* model);
	void Update();
	void OnCollision(const CollisionInfo& info) override;
};

