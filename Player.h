#pragma once
#include "Model.h"

class Player :public Model
{
private:
	Vector2D frontVec;
public:
	void PlayerInitialize();
	void Update();
};

