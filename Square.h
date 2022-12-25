#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"

class UISquare :public VertIdxBuff
{
private:
	GPipeline pipeline;
	UINT vertexSize;
public:
	ScreenVertex pv[4];
	UISquare(ID3D12Device* dev, Shader shader);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle);
private:
	void SetVertices() override;
};

class Square :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	Matrix matWorld;

	Matrix matScale;
	Vector3D scale;

	Matrix matRot;
	Vector3D rotAngle;

	Matrix matTrans;
	Vector3D trans;

	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};

	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;

	GPipeline pipeline;
	Vertex pv[4];
	UINT vertexSize;
	UINT indexSize;
	unsigned short indices[6];
public:
	void SquareInit(ID3D12Device* dev, Shader shader, int blendMord = GPipeline::NONE_BLEND);
	Square();
	Square(ID3D12Device* dev, Shader shader, int blendMord = GPipeline::NONE_BLEND);
	void MatUpdate(Matrix matView, Matrix matProjection);
	void MatUpdate(Matrix matView, Matrix matProjection, Matrix billboard);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle);
private:
	void SetVertices() override;
	void SetMatScaling();
	void SetMatRotation();
	void SetMatTransform();
};

