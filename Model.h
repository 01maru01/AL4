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

	static MyDirectX* dx;

	Material mtl;
public:
	struct ConstBufferDataMaterial {
		Vector3D ambient;
		float pad1;
		Vector3D diffuse;
		float pad2;
		Vector3D specular;
		float alpha;
	};
	ComPtr<ID3D12Resource> material;

	UINT vertexSize;
	std::vector<Vertex> vertices;
	//UINT indexSize;
	//std::vector<unsigned short> indices;

	int textureHandle;

	void Initialize(const char* filename, bool smoothing);
public:
	Model() {};
	Model(const char* filename, bool smoothing = false);
	void Draw();
private:
	void SetVertices() override;
};

