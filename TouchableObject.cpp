#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

TouchableObject* TouchableObject::Create(Model* model)
{
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(Model* model)
{
	Object3D::Initialize();

	SetModel(model);

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	collider->ConstructTriangles(this->GetModel());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	SetCollider(collider);

	return true;
}
