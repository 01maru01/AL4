#pragma once
#include <forward_list>

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
};

