#include "Light.h"

Light* Light::GetInstance()
{
	static Light* instance = new Light;
	return instance;
}

void Light::DeleteInstance()
{
	delete Light::GetInstance();
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferLightData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor;

		for (int i = 0; i < DirLightNum; i++)
		{
			if (dirLights[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			else {
				constMap->dirLights[i].active = 0;
			}
		}

		for (int i = 0; i < PointLightNum; i++)
		{
			if (pointLights[i].IsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].pos = pointLights[i].GetLightPos();
				constMap->pointLights[i].color = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = pointLights[i].GetLightAtten();
			}
			else {
				constMap->pointLights[i].active = 0;
			}
		}

		for (int i = 0; i < SpotLightNum; i++)
		{
			if (spotLights[i].IsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightpos= spotLights[i].GetLightPos();
				constMap->spotLights[i].lightcolor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightv = spotLights[i].GetLightDir();
				constMap->spotLights[i].lightfactoranglecos = spotLights[i].GetLightFactorAngleCos();
			}
			else {
				constMap->spotLights[i].active = 0;
			}
		}

		for (int i = 0; i < CircleShadowNum; i++)
		{
			if (circleShadows[i].IsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos= circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight= circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos= circleShadows[i].GetFactorAngleCos();
			}
			else {
				constMap->circleShadows[i].active = 0;
			}
		}

		constBuff->Unmap(0, nullptr);
	}
}

void Light::Initialize()
{
	//	ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Width = (sizeof(ConstBufferLightData) + 0xFF) & ~0xFF;
	//	生成
	HRESULT result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void Light::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetActive(active);
}

void Light::SetDirLightDir(int index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir(lightdir_);
	dirty = true;
}

void Light::SetDirLightColor(int index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightcolor_);
	dirty = true;
}

void Light::Update()
{
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw()
{
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
}

void  Light::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetActive(active);
}

void  Light::SetPointLightPos(int index, const Vector3D& lightpos)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetLightPos(lightpos);
	dirty = true;
}

void  Light::SetPointLightColor(int index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetLightColor(lightcolor_);
	dirty = true;
}

void  Light::SetPointLightAtten(int index, const Vector3D& lightAtten)
{
	assert(0 <= index && index < PointLightNum);
	pointLights[index].SetLightAtten(lightAtten);
	dirty = true;
}

void Light::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetActive(active);
}

void Light::SetSpotLightDir(int index, const Vector3D& lightdir_)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightDir(lightdir_);
	dirty = true;
}

void Light::SetSpotLightPos(int index, const Vector3D& lightpos)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightPos(lightpos);
	dirty = true;
}

void Light::SetSpotLightColor(int index, const Vector3D& lightcolor_)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightColor(lightcolor_);
	dirty = true;
}

void Light::SetSpotLightAtten(int index, const Vector3D& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightAtten(lightAtten);
	dirty = true;
}

void Light::SetSpotLightFactorAngle(int index, const Vector2D& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void Light::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows[index].SetActive(active);
}

void Light::SetCircleShadowCasterPos(int index, const Vector3D& casterPos_)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows[index].SetCasterPos(casterPos_);
	dirty = true;
}

void Light::SetCircleShadowDir(int index, const Vector3D& dir_)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows[index].SetDir(dir_);
	dirty = true;
}

void Light::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
	dirty = true;
}

void Light::SetCircleShadowAtten(int index, const Vector3D& atten)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows[index].SetAtten(atten);
	dirty = true;
}
void Light::SetCircleShadowFactorAngle(int index, const Vector2D& factorAngle)
{
	assert(0 <= index && index < CircleShadowNum);
	circleShadows[index].SetFactorAngle(factorAngle);
	dirty = true;
}