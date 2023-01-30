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

	BaseCollider* collider = nullptr;
public:
	MyMath::ObjMatrix mat;
	Object3D() = default;
	virtual ~Object3D();

	static Object3D* Create(Model* model = nullptr);

	static void SetLight(Light* light_);
	static void SetPipeline(GPipeline* pipeline_);
	static void SetCamera(ICamera* camera_);
	void SetModel(Model* model_);
	void SetCollider(BaseCollider* collider_);
	void SetPosition(const Vector3D& vec) { mat.trans = vec; }
	void SetAttribute(unsigned short attribute);

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate();
	virtual void Draw();

	inline Model* GetModel() { return model; }
	const Matrix& GetMatWorld() { return mat.matWorld; }

	virtual void OnCollision(const CollisionInfo& info) { (void)info; };
};

