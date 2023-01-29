#pragma once
#include "Vector3D.h"
#include "DirectX.h"

#include "PointLight.h"
#include "DirLight.h"

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 3;
	static const int PointLightNum = 3;

	ComPtr<ID3D12Resource> constBuff;

	Vector3D ambientColor = { 1,1,1 };
	PointLight pointLights[PointLightNum];
	DirLight dirLights[DirLightNum];
	bool dirty = false;
public:
	struct ConstBufferLightData
	{
		Vector3D ambientColor;
		float pad1;
		DirLight::ConstBuffData dirLights[DirLightNum];
		PointLight::ConstBuffData pointLights[PointLightNum];
	};

	static Light* Create();

	void Initialize();
	void TransferConstBuffer();
	void Update();
	void Draw();

	void SetDirLightActive(int index, bool active);
	void SetDirLightDir(int index, const Vector3D& lightdir_);
	void SetDirLightColor(int index, const Vector3D& lightcolor_);
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const Vector3D& lightpos);
	void SetPointLightColor(int index, const Vector3D& lightcolor_);
	void SetPointLightAtten(int index, const Vector3D& lightAtten);
};

