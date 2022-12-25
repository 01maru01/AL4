#include "Model.h"
#include "ObjFile.h"

MyDirectX* Model::dx = MyDirectX::GetInstance();

void Model::Initialize(const char* filename, bool smoothing)
{
	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//	�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = material->Map(0, nullptr, (void**)&constMapMaterial);	//	�}�b�s���O
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = mtl.ambient;
	constMapMaterial->diffuse = mtl.diffuse;
	constMapMaterial->specular = mtl.specular;
	constMapMaterial->alpha = mtl.alpha;
	material->Unmap(0, nullptr);
#pragma endregion

	ObjFile objfile(filename, vertices, mtl, smoothing);

	textureHandle = dx->LoadTextureGraph(mtl.wfilepath);
	vertexSize = vertices.size();
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);

	BuffInitialize(dx->GetDev(), sizeVB, vertexSize);
}

Model::Model(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

void Model::Draw()
{
	BuffUpdate(dx->GetCmdList());
	//	�e�N�X�`��
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(textureHandle));

	dx->GetCmdList()->DrawInstanced(vertexSize, 1, 0, 0);
}

void Model::SetVertices()
{
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
}
