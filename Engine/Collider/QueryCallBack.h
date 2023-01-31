#pragma once
#include "Vector3D.h"
#include "Object3D.h"

struct QueryHit
{
	Object3D* object = nullptr;
	BaseCollider* collider = nullptr;
	Vector3D inter;
	Vector3D reject;
};


class QueryCallBack
{
public:
	QueryCallBack() = default;
	virtual ~QueryCallBack() = default;

	virtual bool OnQueryHit(const QueryHit& info) = 0;
};