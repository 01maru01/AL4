#include "Object3D.h"
#include <cmath>
#include "MyMath.h"
#include <cassert>

void Object3D::Initialize(ID3D12Device* dev, Shader shader)
{
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
#pragma endregion
	vertexSize = 24;
	vertices.resize(vertexSize);
	// 頂点データ
	vertices = {
		//	前
		{{ -1.0f,-1.0f,-1.0f },{},{0.0f, 1.0f}}, // 左下
		{{ -1.0f, 1.0f,-1.0f },{},{0.0f, 0.0f}}, // 左上
		{{  1.0f,-1.0f,-1.0f },{},{1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f,-1.0f },{},{1.0f, 0.0f}}, // 右上
		//	後
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f, 1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f,-1.0f, 1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	左
		{{ -1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{ -1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{ -1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	右
		{{  1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{  1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	下
		{{ -1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f,-1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f,-1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	上
		{{ -1.0f, 1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f, 1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
	};
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);

	indexSize = 36;
	//	インデックスデータ
	indices =
	{
		0,1,2,
		2,1,3,
		6,5,4,
		7,5,6,
		8,9,10,
		10,9,11,
		14,13,12,
		15,13,14,
		18,17,16,
		19,17,18,
		20,21,22,
		22,21,23,
	};
	//	全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(dev, sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout));
#pragma region  WorldMatrix初期値
	scale = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle = Vector3D(0.0f, 0.0f, 0.0f);
	trans = Vector3D(0.0f, 0.0f, 0.0f);
#pragma endregion
}

Object3D::Object3D()
{
}

Object3D::Object3D(ID3D12Device* dev, Shader shader)
{
	Initialize(dev, shader);
}


void Object3D::MatUpdate(Matrix matView, Matrix matProjection)
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

void Object3D::SetVertices()
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

void Object3D::SetMatScaling()
{
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
}

void Object3D::SetMatRotation()
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

void Object3D::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

void Object3D::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}
