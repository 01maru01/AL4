#include "SphereCollider.h"

SphereCollider::SphereCollider(Vector3D offset, float radius) :offset(offset)
{
	this->radius = radius;
	shapeType = COLLISIONSHAPE_SPHERE;
}

void SphereCollider::Update()
{
	const Matrix matWorld = object3d->GetMatWorld();

	Sphere::center.x = matWorld.m[3][0] + offset.x;
	Sphere::center.y = matWorld.m[3][1] + offset.y;
	Sphere::center.z = matWorld.m[3][2] + offset.z;

	Sphere::radius = radius;
}
