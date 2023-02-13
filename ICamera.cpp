#include "ICamera.h"

void ICamera::SetProjection(float fovY, float nearZ, float farZ, int width_, int height_)
{
	matProjection = MyMath::PerspectiveFovLH(width_, height_, fovY, nearZ, farZ);
}

void ICamera::CalcBillboard()
{
#pragma region billboard
	billboard.Identity();
	billboard.m[0][0] = rightVec.x;
	billboard.m[0][1] = rightVec.y;
	billboard.m[0][2] = rightVec.z;
	billboard.m[1][0] = -downVec.x;
	billboard.m[1][1] = -downVec.y;
	billboard.m[1][2] = -downVec.z;
	billboard.m[2][0] = frontVec.x;
	billboard.m[2][1] = frontVec.y;
	billboard.m[2][2] = frontVec.z;
#pragma endregion

#pragma region billboardY
	billboardY.Identity();
	billboardY.m[0][0] = rightVec.x;
	billboardY.m[0][1] = rightVec.y;
	billboardY.m[0][2] = rightVec.z;
	Vector3D billYvecZ = rightVec.cross(up);
	billboardY.m[2][0] = billYvecZ.x;
	billboardY.m[2][1] = billYvecZ.y;
	billboardY.m[2][2] = billYvecZ.z;
#pragma endregion
}

void ICamera::CalcCameraDirVec()
{
	frontVec = target - eye;
	frontVec.normalize();
	rightVec = Vector3D(0, 1, 0).cross(frontVec);
	downVec = rightVec.cross(frontVec);
	rightVec.normalize();
	downVec.normalize();
}

void ICamera::MatUpdate()
{
	matView = MyMath::LookAtLH(eye, target, up);
}
