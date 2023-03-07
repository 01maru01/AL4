#pragma once
#include "MyMath.h"

class Wind
{
private:
	static float time;
	static Vector3D dir;
	static Vector3D force;

	float timer;
public:
	void Initialize();
	void Update();

	void SetTime(float time_) { time = time_; }
	float GetTime() { return timer; }

	static void SetDir(const Vector3D& windDir) { dir = windDir; }
	const Vector3D& GetDir() { return dir; }

	static void SetForce(const Vector3D& windForce) { force = windForce; }
	const Vector3D& GetForce() { return force; }
};

