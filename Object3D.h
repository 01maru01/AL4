#pragma once
#include "Model.h"
#include "Shader.h"
#include "DirectX.h"
#include "ICamera.h"
#include "CollisionInfo.h"

class ICollider;

class Object3D
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct ConstBufferDataTransform {
		Matrix matview;
		Matrix matworld;
		Vector3D cameraPos;
	};
	struct ConstBufferDataMaterial {
		Vector3D ambient;
		float pad1;
		Vector3D diffuse;
		float pad2;
		Vector3D specular;
		float alpha;
	};
private:
	static GPipeline pipeline;
	static Light* light;
	static MyDirectX* dx;
	static ICamera* camera;

	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	Vector4D color;

	MyMath::ObjMatrix mat;
	Model* model = nullptr;
	Object3D* parent = nullptr;
	bool isBillboard = false;
protected:
	ICollider* collider = nullptr;
public:
	Object3D() = default;
	virtual ~Object3D();
	static Object3D* Create(Model* model = nullptr);
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	const Matrix& GetMatWorld() { return mat.matWorld; }

	void SetModel(Model* model) { this->model = model; }
	void SetCollider(ICollider* collider);

	virtual void OnCollision(const CollisionInfo& info) {};
};

