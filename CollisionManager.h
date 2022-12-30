#pragma once
#include <forward_list>
#include "CollisionPrimitive.h"
#include "RayCast.h"
#include <d3d12.h>

class BaseCollider;

class CollisionManager
{
private:
	std::forward_list<BaseCollider*> colliders;

	CollisionManager() {};
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;
public:
	static CollisionManager* GetInstance();
	
	void CheckAllCollisions();
	void AddCollider(BaseCollider* collider) { colliders.push_front(collider); }
	void RemoveCollider(BaseCollider* collider) { colliders.remove(collider); }

	bool Raycast(const Ray& ray, RayCast* hitinfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);
};
