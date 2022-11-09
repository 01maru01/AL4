#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertBuff.h"
#include "Material.h"

class Model :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	MyDirectX* dx = nullptr;
	GPipeline* pipeline = nullptr;

	Material mtl;
public:
	MyMath::ObjMatrix mat;

	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	struct ConstBufferDataMaterial {
		Vector3D ambient;
		float pad1;
		Vector3D diffuse;
		float pad2;
		Vector3D specular;
		float alpha;
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	UINT vertexSize;
	std::vector<Vertex> vertices;
	//UINT indexSize;
	//std::vector<unsigned short> indices;

	void Initialize(Shader shader, const char* filename);
public:
	Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_);
	void MatUpdate(Matrix matView, Matrix matProjection);
	void Draw(int handle);
private:
	void SetVertices() override;
};

