#pragma once
#include "Vector3D.h"
#include "DirectX.h"

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> constBuff;
	Vector3D lightdir = { 1,0,0 };
	Vector3D lightcolor = { 1,1,1 };
	bool dirty = false;
public:
	struct ConstBufferLightData
	{
		Vector3D lightv;
		Vector3D lightcolor;
	};

	static Light* Create();

	void Initialize();
	void TransferConstBuffer();
	void SetLightDir(const Vector3D& lightdir);
	void SetLightColor(const Vector3D& lightcolor);
	void Update();
	void Draw();
};

