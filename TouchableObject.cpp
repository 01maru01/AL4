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

bool TouchableObject::Initialize(Model* model_)
{
	Object3D::Initialize();

	SetModel(model_);

	// コライダーの追加
	MeshCollider* collider_ = new MeshCollider;
	collider_->ConstructTriangles(this->GetModel());
	collider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	SetCollider(collider_);

	return true;
}
