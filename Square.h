#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "ICamera.h"
#include "ParticleCommon.h"

class Particle :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
	static ICamera* camera;
	static ParticleCommon* common;

	struct ConstBufferDataTransform {
		Matrix mat;
		Matrix matBillboard;
		float scale;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;

	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;

	Vector3D vertex;

	float scale = 1.0f;
public:
	static void SetCamera(ICamera* camera);

	void Initialize();
	Particle();
	void MatUpdate();
	void Draw(int handle);
private:
	void SetVertices() override;
};

