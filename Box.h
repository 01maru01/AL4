#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "DirectX.h"
#include "VertIdxBuff.h"
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")

class Box :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	MyDirectX* dx = nullptr;
	GPipeline* pipeline = nullptr;
public:
	MyMath::ObjMatrix mat;

	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};

	UINT vertexSize;
	std::vector<Vertex> vertices;
	UINT indexSize;
	std::vector<unsigned short> indices;

	Box* parent = nullptr;
	void Initialize(Shader shader);
public:
	Box();
	Box(MyDirectX* dx_, GPipeline* pipeline_, Shader shader);
	void MatUpdate(Matrix matView, Matrix matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle);

private:
	void SetVertices() override;
};

