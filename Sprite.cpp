#include "Sprite.h"
#include "DirectX.h"

void Sprite::Initialize()
{
	common = SpriteCommon::GetInstance();

	HRESULT result;

	float left = (0.0f - anchorPoint.x) * size.x;
	float right = (1.0f - anchorPoint.x) * size.x;
	float top = (0.0f - anchorPoint.y) * size.y;
	float bottom = (1.0f - anchorPoint.y) * size.y;

	if (isFlipX) {
		left = -left;
		right = -right;
	}
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top,0.0f };
	vertices[LB].uv = { 0.0f,1.0f };
	vertices[LT].uv = { 0.0f,0.0f };
	vertices[RB].uv = { 1.0f,1.0f };
	vertices[RT].uv = { 1.0f,0.0f };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	VBInitialize(MyDirectX::GetInstance()->GetDev(), sizePV, vertexSize);

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

	//	�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	����
	result = MyDirectX::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	result = material->Map(0, nullptr, (void**)&mapMaterial);	//	�}�b�s���O
	assert(SUCCEEDED(result));

	color = { 1.0f,1.0f,1.0f,1.0f };
}

Sprite::Sprite()
{
	Initialize();
}

void Sprite::MatUpdate()
{
#pragma region WorldMatrix
	matWorld.Identity();

	//	��]
	SetMatRotation();
	matWorld *= matRot;

	//	���s�ړ�
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= common->Get2DMat();

	mapMaterial->color = color;
}

void Sprite::Draw(int handle)
{
	if (isInvisible) {
		return;
	}
	common->Draw();
	VertBuffUpdate(MyDirectX::GetInstance()->GetCmdList());
	//	�e�N�X�`��
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootDescriptorTable(1, MyDirectX::GetInstance()->GetTextureHandle(handle));
	MyDirectX::GetInstance()->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	MyDirectX::GetInstance()->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SetVertices()
{
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	ScreenVertex* vertMap = nullptr;
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

void Sprite::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle);
	matRotZ.m[0][1] = sin(rotAngle);
	matRotZ.m[1][0] = -sin(rotAngle);
	matRotZ.m[1][1] = cos(rotAngle);

	matRot = matRotZ;
}

void Sprite::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = 0.0f;
}
