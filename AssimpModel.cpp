#include "AssimpModel.h"
#include "DirectX.h"

AssimpModel::AssimpModel(GPipeline* pipeline_)
{
	pipeline = pipeline_;
}

void AssimpModel::Initialize(const wchar_t* filename)
{
	ImportSettings importSetting = // ���ꎩ�͎̂���̓ǂݍ��ݐݒ�\����
	{
		filename,
		meshes,
		false,
		true // �A���V�A�̃��f���́A�e�N�X�`����UV��V�������]���Ă���ۂ��H�̂œǂݍ��ݎ���UV���W���t�]������
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
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
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

	//Mesh mesh = {}; // ����܂ł̎l�p�`�����邽�߂̃��b�V���\����
	//mesh.Vertices = std::vector<Vertex>(std::begin(vertices), std::end(vertices));
	//mesh.Indices = std::vector<uint32_t>(std::begin(indices), std::end(indices));
	//meshes.clear(); // ������Ɩ��ʊ����邪�A��U�l�p�`�Ŏ��������̂�AssimpLoader�œǂݍ��񂾃��f���̃��b�V������U�N���A
	//meshes.shrink_to_fit(); // ���g���[���ɂ���
	//meshes.push_back(mesh);

	// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	vertices.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		//auto size = sizeof(Vertex) * meshes[i].Vertices.size();
		//auto stride = sizeof(Vertex);
		//auto vertices = meshes[i].Vertices.data();
		//auto pVB = new VertexBuffer(size, stride, vertices);
		//if (!pVB->IsValid())
		//{
		//	printf("���_�o�b�t�@�̐����Ɏ��s\n");
		//	return;
		//}
		for (size_t j = 0; j < meshes[i].Vertices.size(); j++)
		{
			vertices.push_back(meshes[i].Vertices[j]);
		}
	}

	// ���b�V���̐������C���f�b�N�X�o�b�t�@��p�ӂ���
	indices.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		//auto size = sizeof(uint32_t) * meshes[i].Indices.size();
		//auto indices = meshes[i].Indices.data();
		//auto pIB = new IndexBuffer(size, indices);
		//if (!pIB->IsValid())
		//{
		//	printf("�C���f�b�N�X�o�b�t�@�̐����Ɏ��s\n");
		//	return;
		//}

		for (size_t j = 0; j < meshes[i].Indices.size(); j++)
		{
			indices.push_back(meshes[i].Indices[j]);
		}
	}
	vertexSize = vertices.size();
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	//	�S�̂̃T�C�Y
	indexSize = indices.size();
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(MyDirectX::GetInstance()->GetDev(), sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);
#pragma region  WorldMatrix�����l
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

	// ���b�V���̐������C���f�b�N�X���̕`����s����������
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

	MyDirectX::GetInstance()->GetCmdList()->DrawIndexedInstanced(indices.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
}

void AssimpModel::SetVertices()
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
