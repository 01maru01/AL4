#pragma once
#include "Vector3D.h"
#include "DirectX.h"

#include "PointLight.h"
#include "DirLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "DistanceFog.h"

class Light
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;

	ComPtr<ID3D12Resource> constBuff;

	Vector3D ambientColor = { 1,1,1 };
	PointLight pointLights[PointLightNum];
	DirLight dirLights[DirLightNum];
	SpotLight spotLights[SpotLightNum];
	CircleShadow circleShadows[CircleShadowNum];
	DistanceFog distanceFog;
	bool dirty = false;
public:
	struct ConstBufferLightData
	{
		Vector3D ambientColor;
		float pad1;
		DirLight::ConstBuffData dirLights[DirLightNum];
		PointLight::ConstBuffData pointLights[PointLightNum];
		SpotLight::ConstBuffData spotLights[SpotLightNum];
		CircleShadow::ConstBuffData circleShadows[CircleShadowNum];
		DistanceFog::ConstBuffData distanceFog;
	};
private:
	Light() {};
	~Light() {};
public:
	static Light* GetInstance();
	static void DeleteInstance();
	Light(const Light& obj) = delete;
	Light& operator=(const Light& obj) = delete;

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

	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const Vector3D& lightdir_);
	void SetSpotLightPos(int index, const Vector3D& lightpos);
	void SetSpotLightColor(int index, const Vector3D& lightcolor_);
	void SetSpotLightAtten(int index, const Vector3D& lightAtten);
	void SetSpotLightFactorAngle(int index, const Vector2D& lightFactorAngle);

	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const Vector3D& casterPos_);
	void SetCircleShadowDir(int index, const Vector3D& dir_);
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	void SetCircleShadowAtten(int index, const Vector3D& atten);
	void SetCircleShadowFactorAngle(int index, const Vector2D& factorAngle);

	void SetFogActive(bool active) { distanceFog.SetActive(active); }
	void SetFogStart(float start) { distanceFog.SetStart(start); }
	void SetFogEnd(float end) { distanceFog.SetEnd(end); }
	void SetFogNear(float fogNear) { distanceFog.SetNear(fogNear); }
	void SetFogFar(float fogFar) { distanceFog.SetFar(fogFar); }
};

