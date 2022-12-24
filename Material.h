#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include "DirectX.h"
#include "Vector3D.h"

class Material
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	struct ConstBufferDataMaterial {
		Vector3D ambient;
		float pad1;
		Vector3D diffuse;
		float pad2;
		Vector3D specular;
		float alpha;
	};

	static MyDirectX* dx;
public:
	std::string name;
	Vector3D ambient = { 0.3f, 0.3f, 0.3f };
	Vector3D diffuse;
	Vector3D specular;
	float alpha = 1.0f;
	std::string textureFilename;
	wchar_t wfilepath[128];

	ComPtr<ID3D12Resource> material;
	int textureHandle = -1;

public:
	static Material* Create();

	void Update();
private:
	Material() {};
	void Initialize();
};

