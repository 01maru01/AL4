#include "AssimpModel.h"
#include "DirectX.h"

//#include <filesystem>
//namespace fs = std::filesystem;
std::wstring ReplaceExtension(const std::wstring& origin, const wchar_t* ext)
{
	int idx = origin.length();
	int last = origin.find_last_of('.');
	std::wstring ret = origin.substr(0, last + 1);
	wchar_t buf[5];
	std::swprintf(buf, 5, L"%s", ext);
	//std::wstring ext_ = ext;
	ret.append(ext);
	return ret;
	//fs::path p = origin.c_str();
	//return p.replace_extension(ext).c_str();
}

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

	textureHandle.clear();
	textureHandle.resize(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Vertices.reserve(meshes.size());
		meshes[i].Indices.reserve(meshes.size());
		meshes[i].Initialize();

		auto texPath = ReplaceExtension(meshes[i].DiffuseMap, L"tga");
		const wchar_t* wc = texPath.data();
		textureHandle[i] = MyDirectX::GetInstance()->LoadTextureGraph(wc, true);
	}
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
	for (size_t i = 0; i < meshes.size(); i++)
	{
		pipeline->Setting(MyDirectX::GetInstance()->GetCmdList());
		pipeline->Update(MyDirectX::GetInstance()->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		meshes[i].Update();
		//commandList->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
		//MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
		MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, MyDirectX::GetInstance()->GetTextureHandle(textureHandle[i]));
		MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

		MyDirectX::GetInstance()->GetCmdList()->DrawIndexedInstanced(meshes[i].Indices.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
	}
}
