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
		//float pad1;
		//Vector3D color;
	};

	ComPtr<ID3D12Resource> transform;
	Object3D* parent = nullptr;
	Model* model = nullptr;
protected:
	Vector3D color = { 1.0f,1.0f,1.0f };
	BaseCollider* collider = nullptr;
public:
	static const int MAX_BONES = 32;
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
	ComPtr<ID3D12Resource> constBuffSkin;

	MyMath::ObjMatrix mat;
	Object3D() = default;
	virtual ~Object3D();

	static Object3D* Create(Model* model = nullptr);

	static void SetLight(Light* light_);
	static void SetPipeline(GPipeline* pipeline_);
	static void SetCamera(ICamera* camera_);
	void SetModel(Model* model_);
	void SetCollider(BaseCollider* collider_);
	BaseCollider* GetCollider() { return collider; }
	void SetAttribute(unsigned short attribute);

	void SetColor(const Vector3D& color_) { color = color_; }
	const Vector3D& GetColor() { return color; }

	void SetPosition(const Vector3D& pos_) { mat.trans = pos_; }
	const Vector3D& GetPosition() { return mat.trans; }

	void SetScale(const Vector3D& scale_) { mat.scale = scale_; }
	const Vector3D& GetScale() { return mat.scale; }

	void SetRotation(const Vector3D& rot_) { mat.rotAngle = rot_; }
	const Vector3D& GetRotation() { return mat.rotAngle; }

	virtual void Initialize();
	virtual void ColliderUpdate();
	void MatUpdate();
	virtual void Draw();

	float time = 0.0f;
	void PlayAnimation();

	inline Model* GetModel() { return model; }
	const Matrix& GetMatWorld() { return mat.matWorld; }

	virtual void OnCollision(const CollisionInfo& info) { (void)info; }
};

