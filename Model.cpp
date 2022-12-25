#include "Model.h"
#include "ObjFile.h"

MyDirectX* Model::dx = MyDirectX::GetInstance();

void Model::Initialize(const char* filename, bool smoothing)
{
	HRESULT result;

#pragma region  ConstBuffer
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

	//	定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = material->Map(0, nullptr, (void**)&constMapMaterial);	//	マッピング
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = mtl.ambient;
	constMapMaterial->diffuse = mtl.diffuse;
	constMapMaterial->specular = mtl.specular;
	constMapMaterial->alpha = mtl.alpha;
	material->Unmap(0, nullptr);
#pragma endregion

	ObjFile objfile(filename, vertices, mtl, smoothing);

	textureHandle = dx->LoadTextureGraph(mtl.wfilepath);
	vertexSize = vertices.size();
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);

	BuffInitialize(dx->GetDev(), sizeVB, vertexSize);
}

Model::Model(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

void Model::Draw()
{
	BuffUpdate(dx->GetCmdList());
	//	テクスチャ
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(textureHandle));

	dx->GetCmdList()->DrawInstanced(vertexSize, 1, 0, 0);
}

void Model::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}
