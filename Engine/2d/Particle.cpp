#include "Particle.h"
#include "TextureManager.h"

MyDirectX* Particle::dx = MyDirectX::GetInstance();
ICamera* Particle::camera = nullptr;
ParticleCommon* Particle::common = ParticleCommon::GetInstance();

void Particle::SetCamera(ICamera* camera_)
{
	Particle::camera = camera_;
}

void Particle::Initialize()
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	ID3D12Device* dev = dx->GetDev();
#pragma region  ConstBuffer
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
	result = dev->CreateCommittedResource(
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

	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	����
	result = dev->CreateCommittedResource(
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

	cbResourceDesc.Width = (sizeof(ConstBufferDataWind) + 0xFF) & ~0xFF;
	//	����
	result = dev->CreateCommittedResource(
		&cbHeapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&windRes));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	result = windRes->Map(0, nullptr, (void**)&constMapWind);	//	�}�b�s���O
	assert(SUCCEEDED(result));
#pragma endregion
	UINT sizePV = static_cast<UINT>(sizeof(vertex) * 1);

	BuffInitialize(dev, sizePV, 1);

	wind.Initialize();
}

Particle::Particle()
{
	Initialize();
}

Particle::Particle(const Vector3D& pos_)
{
	vertex = pos_;
	Initialize();
}

void Particle::MatUpdate()
{
	wind.Update();
	constMapWind->windDir = wind.GetDir();
	constMapWind->windForce = wind.GetForce();
	constMapWind->elapsedTime = MyMath::ConvertToRad(wind.GetTime());

	constMapTransform->matBillboard = Matrix();
	if (isBillboardY) {
		constMapTransform->matBillboard = camera->GetBillboardY();
	}
	if (isBillboard) {
		constMapTransform->matBillboard = camera->GetBillboard();
	}
	constMapTransform->scale = scale;
	constMapTransform->cameraPos = camera->GetEye();
	constMapTransform->mat = camera->GetViewProj();

	mapMaterial->color = color;
}

void Particle::Draw(int handle)
{
	ID3D12GraphicsCommandList* cmdList = dx->GetCmdList();

	common->Draw();

	BuffUpdate(cmdList);
	//	�e�N�X�`��
	cmdList->SetGraphicsRootDescriptorTable(0, TextureManager::GetInstance()->GetTextureHandle(handle));
	cmdList->SetGraphicsRootConstantBufferView(1, material->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(3, windRes->GetGPUVirtualAddress());

	cmdList->DrawInstanced(1, 1, 0, 0);
}

void Particle::SetScale(float scale_)
{
	scale = scale_;
}

void Particle::SetPosition(const Vector3D& pos)
{
	vertex = pos;
	SetVertices();
}

void Particle::Move(const Vector3D& spd)
{
	vertex += spd;
	SetVertices();
}

void Particle::SetVertices()
{
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertex);

	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vector3D* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < 1; i++) {
		vertMap[i] = vertex; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
}
