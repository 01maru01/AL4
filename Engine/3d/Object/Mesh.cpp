#include "Mesh.h"
#include "TextureManager.h"

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
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(mtl->GetTextureHandle()));

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
		//	�{�[����weight��0��������l���
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
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	FBXVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
}
