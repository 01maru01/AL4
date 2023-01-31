#include "ScreenPolygon.h"
#include "DirectX.h"

ScreenPolygon::ScreenPolygon()
{
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
#pragma region  ConstBuffer
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	HRESULT result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	SetColor(color);
#pragma endregion

	vertices.clear();
	vertices.push_back({ {-1.0f,-1.0f,0.0f},{0,1} });
	vertices.push_back({ {-1.0f, 1.0f,0.0f},{0,0} });
	vertices.push_back({ { 1.0f,-1.0f,0.0f},{1,1} });
	vertices.push_back({ { 1.0f, 1.0f,0.0f},{1,0} });

	UINT sizePV = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	indexSize = 6;
	//	インデックスデータ
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	BuffInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, sizeIB, indices, indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	Shader shader;
	shader.Initialize(L"Resources/shader/ScreenVS.hlsl", L"Resources/shader/ScreenPS.hlsl");
	pipeline.Init(shader, inputLayout, _countof(inputLayout), 1, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
}

void ScreenPolygon::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline.Setting();
	pipeline.Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, MyDirectX::GetInstance()->GetTextureHandle(0));
	cmdList->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}

void ScreenPolygon::SetColor(const Vector4D& color_)
{
	ConstBufferDataMaterial* mapMaterial = nullptr;
	HRESULT result = material->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	mapMaterial->color = color_;
	assert(SUCCEEDED(result));
	material->Unmap(0, nullptr);
}

void ScreenPolygon::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}
