#include "Model.h"
#include "ObjFile.h"

void Model::Initialize(Shader shader, const char* filename)
{
	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	//	�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	����
	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	result = transform->Map(0, nullptr, (void**)&constMapTransform);	//	�}�b�s���O
	assert(SUCCEEDED(result));
#pragma endregion
	ObjFile objfile(filename, vertices);
	vertexSize = vertices.size();
	//indexSize = indices.size();
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	//	�S�̂̃T�C�Y
	//UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);

	VBInitialize(dx->GetDev(), sizeVB, vertexSize);
#pragma region  WorldMatrix�����l
	mat.Initialize();
#pragma endregion
}

Model::Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_)
{
	dx = dx_;
	pipeline = pipeline_;
	Initialize(shader,filename);
}

void Model::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	mat.Update(matView, matProjection);
#pragma endregion

	constMapTransform->mat = mat.matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Model::Draw(int handle)
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx->GetCmdList());
	//	�e�N�X�`��
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(handle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	dx->GetCmdList()->DrawInstanced(vertexSize, 1, 0, 0);
	//cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
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
