#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertIdxBuff.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"

class Model :public VertIdxBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	//GPipeline* pipeline = nullptr;

	static MyDirectX* dx;
	static GPipeline* pipeline;
	static Light* light;

	Material* mtl = nullptr;
	std::vector<Material> materials;
	std::vector<Mesh*> meshes;
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
	void LoadModel(const std::string& filename, bool smoothing = false);
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	void SetVertices() override;
};

