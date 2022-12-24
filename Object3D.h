#pragma once
#include "GPipeline.h"
#include "Light.h"
#include "MyMath.h"
#include "Model.h"
#include "ICamera.h"
#include "CollisionInfo.h"

class BaseCollider;

class Object3D
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static GPipeline* pipeline;
	static Light* light;
	static MyDirectX* dx;
	static ICamera* camera;

	struct ConstBufferDataTransform {
		Matrix matview;
		Matrix matworld;
		Vector3D cameraPos;
	};



	ComPtr<ID3D12Resource> transform;
	Object3D* parent = nullptr;
	Model* model = nullptr;
protected:
	MyMath::ObjMatrix mat;

	BaseCollider* collider = nullptr;
public:
	Object3D() = default;
	virtual ~Object3D();

	static Object3D* Create(Model* model = nullptr);

	static void SetLight(Light* light);
	static void SetPipeline(GPipeline* pipeline_);
	static void SetCamera(ICamera* camera);
	void SetModel(Model* model);
	void SetCollider(BaseCollider* collider);

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate();
	virtual void Draw();

	const Matrix& GetMatWorld() { return mat.matWorld; }
	virtual void OnCollision(const CollisionInfo& info) {};
};

