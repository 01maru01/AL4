#include "Material.h"

MyDirectX* Material::dx = MyDirectX::GetInstance();

Material* Material::Create()
{
	Material* instance = new Material;
	instance->Initialize();

	return instance;
}

void Material::Update()
{
	// 定数バッファへデータ転送
	ConstBufferDataMaterial* constMaterialMap = nullptr;
	HRESULT result = material->Map(0, nullptr, (void**)&constMaterialMap);
	if (SUCCEEDED(result)) {
		constMaterialMap->ambient = ambient;
		constMaterialMap->diffuse = diffuse;
		constMaterialMap->specular = specular;
		constMaterialMap->alpha = alpha;
		material->Unmap(0, nullptr);
	}
}

void Material::Initialize()
{
	// 生成
	HRESULT result = dx->GetDev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	if (FAILED(result)) {
		assert(0);
	}

	textureHandle = dx->LoadTextureGraph(wfilepath);
}
