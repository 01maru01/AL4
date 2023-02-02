#include "TriangleCollider.h"

TriangleCollider::TriangleCollider(Vector3D p0_, Vector3D p1_, Vector3D p2_, Vector3D normal_)
{
	p0 = p0_;
	p1 = p1_;
	p2 = p2_;
	normal = normal_;
	shapeType = COLLISIONSHAPE_TRIANGLE;
}

void TriangleCollider::Update()
{
	const Matrix matWorld = object3d->GetMatWorld();

	TriangleCollider::normal.x = matWorld.m[3][0] + normal.x;
	TriangleCollider::normal.y = matWorld.m[3][1] + normal.y;
	TriangleCollider::normal.z = matWorld.m[3][2] + normal.z;

	TriangleCollider::p0.x = matWorld.m[3][0] + p0.x;
	TriangleCollider::p0.y = matWorld.m[3][1] + p0.y;
	TriangleCollider::p0.z = matWorld.m[3][2] + p0.z;

	TriangleCollider::p1.x = matWorld.m[3][0] + p1.x;
	TriangleCollider::p1.y = matWorld.m[3][1] + p1.y;
	TriangleCollider::p1.z = matWorld.m[3][2] + p1.z;

	TriangleCollider::p2.x = matWorld.m[3][0] + p2.x;
	TriangleCollider::p2.y = matWorld.m[3][1] + p2.y;
	TriangleCollider::p2.z = matWorld.m[3][2] + p2.z;
}
