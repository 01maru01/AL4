#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "ParticleCommon.h"
#include "ICamera.h"

class Particle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static ParticleCommon* common;
	static MyDirectX* dx;
	static ICamera* camera;
public:
	Matrix matRot;
	Vector3D rotation;

	struct ConstBufferDataTransform {
		Matrix mat;
		Matrix matBillboard;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;

	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;

	ParticleVertex vertices;

	//Vector3D position;
	//float scale = 100.0f;
public:
	static void SetCamera(ICamera* camera);

	void Initialize();
	Particle();
	void MatUpdate();
	void Draw(int handle);
private:
	void SetVertices() override;
	void SetMatRotation();
};

