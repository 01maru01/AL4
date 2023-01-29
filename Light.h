#pragma once
#include "Vector3D.h"
#include "DirectX.h"

#include "PointLight.h"
#include "DirLight.h"

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 1;
	static const int PointLightNum = 3;

	ComPtr<ID3D12Resource> constBuff;
	PointLight pointLights[PointLightNum];
	Vector3D ambientColor = { 1,1,1 };
	Vector3D lightdir = { 1,0,0 };
	Vector3D lightcolor = { 1,1,1 };
	bool dirty = false;
public:
	struct DirLightData
	{
		Vector3D lightv;
		float pad1;
		Vector3D lightcolor;
	};
	struct ConstBufferLightData
	{
		Vector3D ambientColor;
		float pad1;
		DirLightData dirLights[DirLightNum];
		PointLight::ConstBuffData pointLights[PointLightNum];
	};

	static Light* Create();

	void Initialize();
	void TransferConstBuffer();
	void SetLightDir(const Vector3D& lightdir_);
	void SetLightColor(const Vector3D& lightcolor_);
	void Update();
	void Draw();

	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const Vector3D& lightpos);
	void SetPointLightColor(int index, const Vector3D& lightcolor_);
	void SetPointLightAtten(int index, const Vector3D& lightAtten);
};

