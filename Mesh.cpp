#include "Mesh.h"

MyDirectX* Mesh::dx = MyDirectX::GetInstance();

void Mesh::CreateBuff()
{
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	//	全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
	BuffInitialize(sizeVB, vertices.size(), sizeIB, &indices.front(), indices.size());
}

void Mesh::Draw()
{
	BuffUpdate();
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(material->textureHandle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->material->GetGPUVirtualAddress());

	dx->GetCmdList()->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Mesh::AddSmoothData(unsigned short index, unsigned short vertex)
{
	smoothData[index].emplace_back(vertex);
}

void Mesh::CalcSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		std::vector<unsigned short>& v = itr->second;
		Vector3D normal;
		for (unsigned short index : v) {
			normal += vertices[index].normal;
		}
		normal /= (float)v.size();
		normal.normalize();
		for (unsigned short index : v) {
			vertices[index].normal = normal;
		}
	}
}

void Mesh::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
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
