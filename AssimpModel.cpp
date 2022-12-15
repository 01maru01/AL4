#include "AssimpModel.h"
#include "DirectX.h"

//#include <filesystem>
//namespace fs = std::filesystem;
std::wstring ReplaceExtension(const std::wstring& origin, const wchar_t* ext)
{
	int idx = origin.length();
	int last = origin.find_last_of('.');
	std::wstring ret = origin.substr(0, last + 1);
	wchar_t buf[5];
	std::swprintf(buf, 5, L"%s", ext);
	//std::wstring ext_ = ext;
	ret.append(ext);
	return ret;
	//fs::path p = origin.c_str();
	//return p.replace_extension(ext).c_str();
}

AssimpModel::AssimpModel(GPipeline* pipeline_)
{
	pipeline = pipeline_;
}

void AssimpModel::Initialize(const wchar_t* filename)
{
	ImportSettings importSetting = // これ自体は自作の読み込み設定構造体
	{
		filename,
		meshes,
		false,
		true // アリシアのモデルは、テクスチャのUVのVだけ反転してるっぽい？ので読み込み時にUV座標を逆転させる
	};

	AssimpLoader loader;
	//loader.Load(importSetting);
	if (!loader.Load(importSetting))
	{
		return;
	}

	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = transform->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(result));
#pragma endregion

	textureHandle.clear();
	textureHandle.resize(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Vertices.reserve(meshes.size());
		meshes[i].Indices.reserve(meshes.size());
		meshes[i].Initialize();

		auto texPath = ReplaceExtension(meshes[i].DiffuseMap, L"tga");
		const wchar_t* wc = texPath.data();
		textureHandle[i] = MyDirectX::GetInstance()->LoadTextureGraph(wc, true);
	}
#pragma region  WorldMatrix初期値
	mat.Initialize();
#pragma endregion
}

void AssimpModel::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	mat.Update();
#pragma endregion

	constMapTransform->mat = mat.matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void AssimpModel::Draw()
{
	auto commandList = MyDirectX::GetInstance()->GetCmdList();

	// メッシュの数だけインデックス分の描画を行う処理を回す
	for (size_t i = 0; i < meshes.size(); i++)
	{
		pipeline->Setting(MyDirectX::GetInstance()->GetCmdList());
		pipeline->Update(MyDirectX::GetInstance()->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		meshes[i].Update();
		//commandList->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
		//MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
		MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, MyDirectX::GetInstance()->GetTextureHandle(textureHandle[i]));
		MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

		MyDirectX::GetInstance()->GetCmdList()->DrawIndexedInstanced(meshes[i].Indices.size(), 1, 0, 0, 0); // インデックスの数分描画する
	}
}
