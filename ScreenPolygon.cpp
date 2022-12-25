#include "ScreenPolygon.h"
#include "DirectX.h"

ScreenPolygon::ScreenPolygon()
{
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
	BuffInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, vertices.size(), sizeIB, indices, indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	shader.Initialize(L"Resources/shader/ScreenVS.hlsl", L"Resources/shader/ScreenPS.hlsl");
	pipeline.Init(MyDirectX::GetInstance()->GetDev(), shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
}

void ScreenPolygon::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, MyDirectX::GetInstance()->GetTextureHandle(0));

	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
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
