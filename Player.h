#pragma once
#include "Model.h"
#include "ICamera.h"

class Player :public Model
{
private:
	Vector2D frontVec;
	float spd;

	static ICamera* camera;
	static const float MAX_SPD;
	static const int INVINCIBLE_TIME;
public:
	static void SetCamera(ICamera* camera_);
	void PlayerInitialize();
	void Update();
};

