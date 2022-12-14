#include "AssimpModel.h"
#include "DirectX.h"

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


	//Vertex vertices[4] = {};
	//vertices[0].Position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	//vertices[0].Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	//vertices[1].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	//vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	//vertices[2].Color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	//vertices[3].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	//vertices[3].Color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	//uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

	//Mesh mesh = {}; // これまでの四角形を入れるためのメッシュ構造体
	//mesh.Vertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));
	//mesh.Indices = std::vector<uint32_t>(std::begin(indices), std::end(indices));
	//meshes.clear(); // ちょっと無駄感あるが、一旦四角形で試したいのでAssimpLoaderで読み込んだモデルのメッシュを一旦クリア
	//meshes.shrink_to_fit(); // 中身をゼロにする
	//meshes.push_back(mesh);

	// メッシュの数だけ頂点バッファを用意する
	vertices.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		//auto size = sizeof(Vertex) * meshes[i].Vertices.size();
		//auto stride = sizeof(Vertex);
		//auto vertices = meshes[i].Vertices.data();
		//auto pVB = new VertexBuffer(size, stride, vertices);
		//if (!pVB->IsValid())
		//{
		//	printf("頂点バッファの生成に失敗\n");
		//	return;
		//}
		for (size_t j = 0; j < meshes[i].Vertices.size(); j++)
		{
			vertices.push_back(meshes[i].Vertices[j]);
		}
	}

	// メッシュの数だけインデックスバッファを用意する
	indices.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		//auto size = sizeof(uint32_t) * meshes[i].Indices.size();
		//auto indices = meshes[i].Indices.data();
		//auto pIB = new IndexBuffer(size, indices);
		//if (!pIB->IsValid())
		//{
		//	printf("インデックスバッファの生成に失敗\n");
		//	return;
		//}

		for (size_t j = 0; j < meshes[i].Indices.size(); j++)
		{
			indices.push_back(meshes[i].Indices[j]);
		}
	}
	vertexSize = vertices.size();
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	//	全体のサイズ
	indexSize = indices.size();
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(MyDirectX::GetInstance()->GetDev(), sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);
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
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//}
	pipeline->Setting(MyDirectX::GetInstance()->GetCmdList());
	pipeline->Update(MyDirectX::GetInstance()->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(MyDirectX::GetInstance()->GetCmdList());
	//commandList->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	//MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	//MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, MyDirectX::GetInstance()->GetTextureHandle(1));
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	MyDirectX::GetInstance()->GetCmdList()->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0); // インデックスの数分描画する
}

void AssimpModel::SetVertices()
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
