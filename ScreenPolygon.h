#pragma once
#include "GPipeline.h"
#include "VertBuff.h"

class ScreenPolygon :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//D3D12_HEAP_PROPERTIES cbHeapProp{};
	//D3D12_RESOURCE_DESC cbResourceDesc{};

	//D3D12_HEAP_PROPERTIES heapProp{};
	//D3D12_RESOURCE_DESC resourceDesc{};
	//struct ConstBufferDataMaterial {
	//	Vector4D color;	//	RGBA
	//};
	//ComPtr<ID3D12Resource> material;
	//ConstBufferDataMaterial* mapMaterial = nullptr;

	GPipeline pipeline;
	Shader shader;
	std::vector<ScreenVertex> vertices;
	UINT indexSize;
	unsigned short indices[6];
public:
	ScreenPolygon();
	void Draw();
private:
	void SetVertices() override;
};

