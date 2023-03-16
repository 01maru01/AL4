#include "Mesh.h"

MyDirectX* Mesh::dx = MyDirectX::GetInstance();

void Mesh::CreateBuff()
{
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
	BuffInitialize(dx->GetDev(), sizeVB, sizeIB, &indices.front(), (int)indices.size());
}

void Mesh::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();
	BuffUpdate(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(0, dx->GetTextureHandle(mtl->GetTextureHandle()));

	ID3D12Resource* constBuff = mtl->GetMaterialConstBuff();
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Mesh::CalcSmoothedNormals()
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

void Mesh::SetBone(int vertexID, UINT boneIndex, float weight)
{
	for (UINT i = 0; i < 4; i++) {
		//	ボーンのweightが0だったら値代入
		if (vertices[vertexID].boneWeight[i] == 0.0f) {
			vertices[vertexID].boneIndex[i] = boneIndex;
			vertices[vertexID].boneWeight[i] = weight;
			return;
		}
	}
}

void Mesh::SetTextureFilePath(const std::string& filePath)
{
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, mtl->wfilepath, _countof(mtl->wfilepath));
}

void Mesh::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	FBXVertex* vertMap = nullptr;
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
