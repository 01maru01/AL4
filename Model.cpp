#include "Model.h"
#include "ObjFile.h"

Light* Model::light = nullptr;

void Model::Initialize(Shader shader, const char* filename, bool smoothing)
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
	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = material->Map(0, nullptr, (void**)&constMapMaterial);	//	マッピング
	assert(SUCCEEDED(result));
#pragma endregion
	ObjFile objfile(filename, vertices, mtl, smoothing);

	constMapMaterial->ambient = mtl.ambient;
	constMapMaterial->diffuse = mtl.diffuse;
	constMapMaterial->specular = mtl.specular;
	constMapMaterial->alpha = mtl.alpha;
	textureHandle = dx->LoadTextureGraph(mtl.wfilepath);
	vertexSize = vertices.size();
	//indexSize = indices.size();
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	//	全体のサイズ
	//UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);

	VBInitialize(dx->GetDev(), sizeVB, vertexSize);
#pragma region  WorldMatrix初期値
	mat.Initialize();
#pragma endregion
}

void Model::SetLight(Light* light)
{
	Model::light = light;
}

Model::Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_, bool smoothing)
{
	dx = dx_;
	pipeline = pipeline_;
	Initialize(shader,filename, smoothing);
}

void Model::MatUpdate(Matrix matView, Matrix matProjection, const Vector3D& cameraPos)
{
#pragma region WorldMatrix
	mat.Update();
#pragma endregion

	constMapTransform->matworld = mat.matWorld;
	constMapTransform->matview = matView;
	constMapTransform->matview *= matProjection;
	constMapTransform->cameraPos = cameraPos;
}

void Model::Draw()
{
	pipeline->Setting(MyDirectX::GetInstance()->GetCmdList());
	pipeline->Update(MyDirectX::GetInstance()->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(MyDirectX::GetInstance()->GetCmdList());
	//	テクスチャ
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(textureHandle));
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	light->Draw();

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(vertexSize, 1, 0, 0);
	//cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
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
