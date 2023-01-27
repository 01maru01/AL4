#include "Light.h"

void Light::TransferConstBuffer()
{
	HRESULT result;

	ConstBufferLightData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::Initialize()
{
	//	�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Width = (sizeof(ConstBufferLightData) + 0xFF) & ~0xFF;
	//	����
	HRESULT result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

Light* Light::Create()
{
	Light* instance = new Light();
	instance->Initialize();
	return instance;
}

void Light::SetLightDir(const Vector3D& lightdir_)
{
	this->lightdir = lightdir_;
	this->lightdir.normalize();
	dirty = true;
}

void Light::SetLightColor(const Vector3D& lightcolor_)
{
	this->lightcolor = lightcolor_;
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
