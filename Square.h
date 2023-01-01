#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "ICamera.h"

class Square :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static MyDirectX* dx;
	static ICamera* camera;

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

	GPipeline pipeline;
	Vector3D vertex;
	UINT vertexSize;
public:
	MyMath::ObjMatrix mat;
public:
	static void SetCamera(ICamera* camera);

	void Initialize(int blendMord = GPipeline::NONE_BLEND);
	Square();
	void MatUpdate();
	void Draw(int handle);
private:
	void SetVertices() override;
};

