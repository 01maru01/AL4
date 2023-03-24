#include "Sprite.h"
#include "DirectX.h"
#include "TextureManager.h"

void Sprite::Initialize(uint32_t handle_)
{
	common = SpriteCommon::GetInstance();
	
	HRESULT result;

	if (handle_ != UINT32_MAX) {
		handle = handle_;
		AdjustTextureSize();
		size = textureSize;
	}

	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (isFlipX) {
		left = -left;
		right = -right;
	}
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}
	vertices.resize(4);
	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };
	vertices[LB].uv = { 0.0f,1.0f };
	vertices[LT].uv = { 0.0f,0.0f };
	vertices[RB].uv = { 1.0f,1.0f };
	vertices[RT].uv = { 1.0f,0.0f };
	UINT sizePV = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	BuffInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, (int)vertices.size());

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

	//	ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = material->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	assert(SUCCEEDED(result));

	color = { 1.0f,1.0f,1.0f,1.0f };
}

void Sprite::Update()
{
	ID3D12Resource* texBuff = TextureManager::GetInstance()->GetTextureBuffer(handle);

	if (texBuff) {
		D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();

		float tex_left = textureLeftTop.x / resDesc_.Width;
		float tex_right = (textureLeftTop.x + textureSize.x) / resDesc_.Width;
		float tex_top = textureLeftTop.y / resDesc_.Height;
		float tex_bottom = (textureLeftTop.y + textureSize.y) / resDesc_.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
		TransferVertex();
	}

	MatUpdate();
}

Sprite::Sprite(uint32_t handle_)
{
	Initialize(handle_);
}

void Sprite::MatUpdate()
{
#pragma region WorldMatrix
	matWorld.Identity();

	//	回転
	SetMatRotation();
	matWorld *= matRot;

	//	平行移動
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= common->Get2DMat();

	mapMaterial->color = color;
}

void Sprite::Draw()
{
	if (isInvisible) {
		return;
	}
	common->Draw();
	BuffUpdate(MyDirectX::GetInstance()->GetCmdList());
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle));
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertex()
{
	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (isFlipX) {
		left = -left;
		right = -right;
	}
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };

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

void Sprite::SetVertices()
{
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
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void Sprite::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle);
	matRotZ.m[0][1] = sin(rotAngle);
	matRotZ.m[1][0] = -sin(rotAngle);
	matRotZ.m[1][1] = cos(rotAngle);

	matRot = matRotZ;
}

void Sprite::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = 0.0f;
}

void Sprite::AdjustTextureSize()
{
	ID3D12Resource* texBuff = TextureManager::GetInstance()->GetTextureBuffer(handle);
	assert(texBuff);

	D3D12_RESOURCE_DESC resDesc_ = texBuff->GetDesc();
	textureSize.x = static_cast<float>(resDesc_.Width);
	textureSize.y = static_cast<float>(resDesc_.Height);
}
