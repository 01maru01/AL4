#pragma once
#include "GPipeline.h"
#include "Light.h"
#include "MyMath.h"
#include "Model.h"
#include "ICamera.h"

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



	MyMath::ObjMatrix mat;
	ComPtr<ID3D12Resource> transform;
	Object3D* parent = nullptr;
	Model* model = nullptr;
public:
	static Object3D* Create(Model* model = nullptr);

	static void SetLight(Light* light);
	static void SetPipeline(GPipeline* pipeline_);
	static void SetCamera(ICamera* camera);
	void SetModel(Model* model);

	void Initialize();
	void Update();
	void Draw();
};

