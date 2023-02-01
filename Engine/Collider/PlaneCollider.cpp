#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(Vector3D normal, float distance_)
{
	this->normal = normal;
	this->distance = distance_;
	shapeType = COLLISIONSHAPE_PLANE;
}

void PlaneCollider::Update()
{
	const Matrix matWorld = object3d->GetMatWorld();

	//Plane::center.x = matWorld.m[3][0] + offset.x;
	//Plane::center.y = matWorld.m[3][1] + offset.y;
	//Plane::center.z = matWorld.m[3][2] + offset.z;

	//Plane::radius = radius;
}
