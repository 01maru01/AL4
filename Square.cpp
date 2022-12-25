#include "Square.h"

UISquare::UISquare(ID3D12Device* dev, Shader shader)
{
	HRESULT result;

	pv[0] = { {-1.0f,-1.0f,0.1f},{0,1} };
	pv[1] = { {-1.0f, 1.0f,0.1f},{0,0} };
	pv[2] = { { 1.0f,-1.0f,0.1f},{1,1} };
	pv[3] = { { 1.0f, 1.0f,0.1f},{1,0} };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	BuffInitialize(dev, sizePV, vertexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout));
	pipeline.SetBlend(dev, GPipeline::ALPHA_BLEND);
}

void UISquare::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(pv[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = pv[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void UISquare::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Square::SquareInit(ID3D12Device* dev, Shader shader, int blendMord)
{
	HRESULT result;

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
	result = dev->CreateCommittedResource(
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
#pragma endregion
	pv[0] = { {-0.5f,-0.5f,0.0f},{},{0,1} };
	pv[1] = { {-0.5f, 0.5f,0.0f},{},{0,0} };
	pv[2] = { { 0.5f,-0.5f,0.0f},{},{1,1} };
	pv[3] = { { 0.5f, 0.5f,0.0f},{},{1,0} };
	pv[0] = { {-1.0f,-1.0f,0.0f},{},{0,1} };
	pv[1] = { {-1.0f, 1.0f,0.0f},{},{0,0} };
	pv[2] = { { 1.0f,-1.0f,0.0f},{},{1,1} };
	pv[3] = { { 1.0f, 1.0f,0.0f},{},{1,0} };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	indexSize = 6;
	//	インデックスデータ
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	BuffInitialize(dev, sizePV, vertexSize, sizeIB, indices, indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	if (blendMord != GPipeline::NONE_BLEND) {
		pipeline.SetBlend(dev, blendMord);
	}
#pragma region  WorldMatrix初期値
	scale = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle = Vector3D(0.0f, 0.0f, 0.0f);
	trans = Vector3D(0.0f, 0.0f, 0.0f);
#pragma endregion
}

Square::Square()
{
}

Square::Square(ID3D12Device* dev, Shader shader, int blendMord)
{
	SquareInit(dev, shader, blendMord);
}

void Square::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	matWorld.Identity();

	//	スケーリング
	SetMatScaling();
	matWorld *= matScale;

	//	回転
	SetMatRotation();
	matWorld *= matRot;

	//	平行移動
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Square::MatUpdate(Matrix matView, Matrix matProjection, Matrix billboard)
{
#pragma region WorldMatrix
	matWorld.Identity();

	matWorld *= billboard;

	//	スケーリング
	SetMatScaling();
	matWorld *= matScale;

	//	回転
	SetMatRotation();
	matWorld *= matRot;

	//	平行移動
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Square::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}

void Square::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(pv[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = pv[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void Square::SetMatScaling()
{
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
}

void Square::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotX;
	matRotX.m[1][1] = cos(rotAngle.x);
	matRotX.m[1][2] = sin(rotAngle.x);
	matRotX.m[2][1] = -sin(rotAngle.x);
	matRotX.m[2][2] = cos(rotAngle.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotAngle.y);
	matRotY.m[2][0] = sin(rotAngle.y);
	matRotY.m[0][2] = -sin(rotAngle.y);
	matRotY.m[2][2] = cos(rotAngle.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle.z);
	matRotZ.m[0][1] = sin(rotAngle.z);
	matRotZ.m[1][0] = -sin(rotAngle.z);
	matRotZ.m[1][1] = cos(rotAngle.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
}

void Square::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}
