#pragma once
#include "Object2D.h"

class VolumeLightObj
{
private:
	static int handle;

	Object2D lightObj[2];
	
public:
	static void SetLightGraph(int handle_);
	void Initialize(const Vector2D& scale_, const Vector3D& pos, const Vector3D& dir);
	void MatUpdate();
	void Draw();
};

