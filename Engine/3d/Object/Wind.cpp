#include "Wind.h"

float Wind::time = 60.0f;
Vector3D Wind::dir = Vector3D(1.0f, 0.0f, 0.0f);
Vector3D Wind::force = Vector3D(0.2f, 0.2f, 0.0f);

void Wind::Initialize()
{
	timer = (float)((int)MyMath::GetRand(0.0f, time));
}

void Wind::Update()
{
	timer++;
	//if (timer > time) timer = 0;
}
