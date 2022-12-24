#include "SphereCollider.h"

void SphereCollider::Update()
{
	const Matrix matWorld = object3d->GetMatWorld();

	center.x = matWorld.m[3][0] + offset.x;
	center.y = matWorld.m[3][1] + offset.y;
	center.z = matWorld.m[3][2] + offset.z;
}
