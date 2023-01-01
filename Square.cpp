#include "Square.h"

MyDirectX* Square::dx = MyDirectX::GetInstance();
ICamera* Square::camera = nullptr;

void Square::SetCamera(ICamera* camera)
{
	Square::camera = camera;
}

void Square::Initialize(int blendMord)
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ID3D12Device* dev = dx->GetDev();
#pragma region  ConstBuffer
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
	result = dev->CreateCommittedResource(
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

	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result = dev->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = material->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	assert(SUCCEEDED(result));
#pragma endregion
	vertex = { 0.0f,0.0f,0.0f };
	vertexSize = 1;
	UINT sizePV = static_cast<UINT>(sizeof(vertex) * vertexSize);

	BuffInitialize(dev, sizePV, vertexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
	};
	Shader shader;
	shader.Initialize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl", "main", L"Resources/shader/BasicGS.hlsl");

	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	if (blendMord != GPipeline::NONE_BLEND) {
		pipeline.SetBlend(dev, blendMord);
	}
#pragma region  WorldMatrix初期値
	mat.Initialize();
#pragma endregion
}

Square::Square()
{
	Initialize();
}

void Square::MatUpdate()
{
#pragma region WorldMatrix
	mat.Update();
#pragma endregion
	Matrix bill;
	constMapTransform->matBillboard = camera->GetBillboardY();
	//constMapTransform->matBillboard = bill;
	constMapTransform->scale = 5.0f;
	constMapTransform->mat = camera->GetViewProj();
}

void Square::Draw(int handle)
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();

	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(handle));
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	cmdList->DrawInstanced(1, 1, 0, 0);
}

void Square::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertex);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vector3D* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = vertex; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}
