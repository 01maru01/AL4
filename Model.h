#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertBuff.h"
#include "Material.h"
#include "Light.h"

class Model :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	//GPipeline* pipeline = nullptr;

	static MyDirectX* dx;
	static GPipeline* pipeline;
	static Light* light;

	Material mtl;
public:
	MyMath::ObjMatrix mat;

	struct ConstBufferDataTransform {
		Matrix matview;
		Matrix matworld;
		Vector3D cameraPos;
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

	int textureHandle;

	void Initialize(const char* filename, bool smoothing);
public:
	static void SetLight(Light* light);
	static void SetPipeline(GPipeline* pipeline_);
public:
	Model() {};
	Model(const char* filename, bool smoothing = false);
	void MatUpdate(Matrix matView, Matrix matProjection, const Vector3D& cameraPos);
	void Draw();
private:
	void SetVertices() override;
};

