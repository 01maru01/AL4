#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "AssimpLoader.h"
#include "VertBuff.h"
#include "GPipeline.h"

class AssimpModel :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	GPipeline* pipeline = nullptr;

	std::vector<Mesh> meshes;
	UINT vertexSize;
	std::vector<Vertex> vertices;
	UINT indexSize;
	std::vector<uint16_t> indices;

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
private:
	void SetVertices() override;
};

