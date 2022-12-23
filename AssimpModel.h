#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "AssimpLoader.h"
#include "VertIdxBuff.h"
#include "GPipeline.h"

class AssimpModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	GPipeline* pipeline = nullptr;

	std::vector<Mesh> meshes;
	std::vector<int> textureHandle;

	MyMath::ObjMatrix mat;

	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
public:
	AssimpModel(GPipeline* pipeline_);
	void Initialize(const wchar_t* filename);
	void MatUpdate(Matrix matView, Matrix matProjection);
	void Draw();
};

