#include "Mesh.h"

MyDirectX* Mesh::dx = MyDirectX::GetInstance();

void Mesh::CreateBuff()
{
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	//	�S�̂̃T�C�Y
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
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
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
