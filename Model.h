#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "VertBuff.h"

class Model :public VertBuff
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

	GPipeline pipeline;
	UINT vertexSize;
	std::vector<Vertex> vertices;
	//UINT indexSize;
	//std::vector<unsigned short> indices;

	void Initialize(ID3D12Device* dev, Shader shader, const char* filename);
public:
	Model(ID3D12Device* dev, Shader shader, const char* filename);
	void MatUpdate(Matrix matView, Matrix matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle);
private:
	void SetVertices() override;
	void SetMatScaling();
	void SetMatRotation();
	void SetMatTransform();
};

