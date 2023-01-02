#include "Material.h"

MyDirectX* Material::dx = MyDirectX::GetInstance();

Material* Material::Create()
{
	Material* instance = new Material;
	instance->Initialize();

	return instance;
}

void Material::Initialize()
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));
}

void Material::Update()
{
	// 定数バッファへデータ転送
	ConstBufferDataMaterial* constMap = nullptr;
	HRESULT result = material->Map(0, nullptr, (void**)&constMap);

	if (SUCCEEDED(result)) {
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		material->Unmap(0, nullptr);
	}
}

std::wstring ReplaceExtension(const std::wstring& origin, const wchar_t* ext)
{
	int idx = origin.length();
	int last = origin.find_last_of('.');
	std::wstring ret = origin.substr(0, last + 1);
	wchar_t buf[5];
	std::swprintf(buf, 5, L"%s", ext);
	ret.append(ext);
	return ret;
}

void Material::LoadTexture(bool tga)
{
	if (tga) {
		auto texPath = ReplaceExtension(wfilepath, L"tga");
		const wchar_t* wc = texPath.data();

		textureHandle = dx->LoadTextureGraph(wc, tga);
	}
	else {
		textureHandle = dx->LoadTextureGraph(wfilepath, tga);
	}
}
