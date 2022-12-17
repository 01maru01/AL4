#include "ScreenPolygon.h"
#include "DirectX.h"

ScreenPolygon::ScreenPolygon()
{
	HRESULT result;

	//pv[0] = { {-0.5f,-0.5f,0.0f},{},{0,1} };
	//pv[1] = { {-0.5f, 0.5f,0.0f},{},{0,0} };
	//pv[2] = { { 0.5f,-0.5f,0.0f},{},{1,1} };
	//pv[3] = { { 0.5f, 0.5f,0.0f},{},{1,0} };
	vertices.clear();
	vertices.push_back({ {-1.0f,-1.0f,0.0f},{},{0,1} });
	vertices.push_back({ {-1.0f, 1.0f,0.0f},{},{0,0} });
	vertices.push_back({ { 1.0f,-1.0f,0.0f},{},{1,1} });
	vertices.push_back({ { 1.0f, 1.0f,0.0f},{},{1,0} });
	
	UINT sizePV = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	indexSize = 6;
	//	�C���f�b�N�X�f�[�^
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, vertices.size(), sizeIB, indices, indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�@���x�N�g��
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};
	shader.Init(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	pipeline.Init(MyDirectX::GetInstance()->GetDev(), shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
}

void ScreenPolygon::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(cmdList);
	//	�e�N�X�`��
	cmdList->SetGraphicsRootDescriptorTable(1, MyDirectX::GetInstance()->GetTextureHandle(0));

	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}

void ScreenPolygon::SetVertices()
{
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

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
}
