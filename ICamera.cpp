#include "ICamera.h"

void ICamera::SetProjection(float fovY, float nearZ, float farZ, int width_, int height_)
{
	matProjection = MyMath::PerspectiveFovLH(width_, height_, fovY, nearZ, farZ);
}

void ICamera::SetMatView()
{
	view.SetMatrix();
}
