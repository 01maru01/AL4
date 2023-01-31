#include "NormalCamera.h"

NormalCamera::NormalCamera()
{
}

NormalCamera::~NormalCamera()
{
}

void NormalCamera::Initialize(Vector3D eye_, Vector3D target_, Vector3D up_)
{
	eye = eye_;
	target = target_;
	up = up_;

	MatUpdate();
}

void NormalCamera::Update()
{
	frontVec = target - eye;
	frontVec.normalize();
	rightVec = Vector3D(0, 1, 0).cross(frontVec);
	downVec = rightVec.cross(frontVec);

#pragma region ビルボード
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

	billboardY.Identity();
	billboardY.m[0][0] = rightVec.x;
	billboardY.m[0][1] = rightVec.y;
	billboardY.m[0][2] = rightVec.z;
	Vector3D billYvecZ = rightVec.cross(up);
	billboardY.m[2][0] = billYvecZ.x;
	billboardY.m[2][1] = billYvecZ.y;
	billboardY.m[2][2] = billYvecZ.z;
#pragma endregion

	MatUpdate();
}
