#include "Particle.h"

ParticleCommon* Particle::common = ParticleCommon::GetInstance();
MyDirectX* Particle::dx = MyDirectX::GetInstance();
ICamera* Particle::camera = nullptr;

void Particle::SetCamera(ICamera* camera)
{
	Particle::camera = camera;
}

void Particle::Initialize()
{
	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
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
	result = dx->GetDev()->CreateCommittedResource(
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
	result = dx->GetDev()->CreateCommittedResource(
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
	UINT sizePV = static_cast<UINT>(sizeof(vertices));
	BuffInitialize(dx->GetDev(), sizePV, 1);
#pragma region  WorldMatrix初期値
	vertices.scale = 10000.0f;
#pragma endregion
}

Particle::Particle()
{
	Initialize();
}

void Particle::MatUpdate()
{
#pragma region WorldMatrix
	//	回転
	SetMatRotation();
#pragma endregion
	constMapTransform->matBillboard = camera->GetBillboard();
	constMapTransform->mat = camera->GetViewProj();
}

void Particle::Draw(int handle)
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();

	common->Draw();

	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(handle));
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	cmdList->DrawInstanced(1, 1, 0, 0);
}

void Particle::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ParticleVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < 1; i++) {
		vertMap[i] = vertices; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void Particle::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotX;
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
}
