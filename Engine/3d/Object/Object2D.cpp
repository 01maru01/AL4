#include "Object2D.h"
#include "TextureManager.h"

MyDirectX* Object2D::dx = MyDirectX::GetInstance();
GPipeline* Object2D::pipeline = nullptr;
ICamera* Object2D::camera = nullptr;

void Object2D::SetVertices()
{
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	ScreenVertex* vertMap = nullptr;
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

Object2D* Object2D::Create()
{
	Object2D* obj = new Object2D();
	if (obj == nullptr) {
		return nullptr;
	}

	// ������
	//obj->ObjInitialize();

	return obj;
}

void Object2D::SetPipeline(GPipeline* pipeline_)
{
	Object2D::pipeline = pipeline_;
}

void Object2D::SetCamera(ICamera* camera_)
{
	Object2D::camera = camera_;
}

void Object2D::ObjInitialize(const Vector2D& anchorPoint)
{
	float left = (0.0f - anchorPoint.x);
	float right = (1.0f - anchorPoint.x);
	float top = (0.0f - anchorPoint.y);
	float bottom = (1.0f - anchorPoint.y);
	vertices.push_back({ { left,top,0.0f},{0,1} });
	vertices.push_back({ { left, bottom,0.0f},{0,0} });
	vertices.push_back({ { right,top,0.0f},{1,1} });
	vertices.push_back({ { right, bottom,0.0f},{1,0} });
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
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


	ConstBufferDataMaterial* mapMaterial = nullptr;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	����
	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	result = material->Map(0, nullptr, (void**)&mapMaterial);	//	�}�b�s���O
	assert(SUCCEEDED(result));

	mapMaterial->color = Vector4D(color, 1.0f);
	material->Unmap(0, nullptr);

	mat.Initialize();

	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	BuffInitialize(dx->GetDev(), sizeVB, sizeIB, &indices.front(), (int)indices.size());
}

void Object2D::MatUpdate()
{
	mat.Update();

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		mat.matWorld *= parent->mat.matWorld;
	}

	ConstBufferDataTransform* constMap = nullptr;
	transform->Map(0, nullptr, (void**)&constMap);
	constMap->mat = mat.matWorld;
	constMap->mat *= camera->GetViewProj();
	transform->Unmap(0, nullptr);
}

void Object2D::Draw(int handle)
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();

	pipeline->Setting();
	pipeline->Update();

	BuffUpdate(cmdList);
	//	�e�N�X�`��
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle));
	cmdList->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
