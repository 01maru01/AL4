#pragma once
#include <string>
#include "Vector3D.h"
#include "DirectX.h"

class Material
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static MyDirectX* dx;
public:
	struct ConstBufferDataMaterial {
		Vector3D ambient;
		float pad1;
		Vector3D diffuse;
		float pad2;
		Vector3D specular;
		float alpha;
	};
public:
	std::string name;
	Vector3D ambient = { 0.3f, 0.3f, 0.3f };
	Vector3D diffuse;
	Vector3D specular;
	float alpha = 1.0f;
	std::string textureFilename;
	wchar_t wfilepath[128];

	int textureHandle = -1;
private:
	ComPtr<ID3D12Resource> material;

public:
	static Material* Create();

	void Initialize();
	void Update();
	void LoadTexture(bool tga = false);

	const int GetTextureHandle() { return textureHandle; }
	ID3D12Resource* GetMaterialConstBuff() { return material.Get(); }
};

