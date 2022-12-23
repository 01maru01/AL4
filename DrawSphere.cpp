#include "DrawSphere.h"

void SphereObj::SetVertices()
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

void SphereObj::SetMatScaling()
{
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
}

void SphereObj::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotX;
	matRotX.m[1][1] = cos(rotAngle.x);
	matRotX.m[1][2] = sin(rotAngle.x);
	matRotX.m[2][1] = -sin(rotAngle.x);
	matRotX.m[2][2] = cos(rotAngle.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotAngle.y);
	matRotY.m[2][0] = sin(rotAngle.y);
	matRotY.m[0][2] = -sin(rotAngle.y);
	matRotY.m[2][2] = cos(rotAngle.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle.z);
	matRotZ.m[0][1] = sin(rotAngle.z);
	matRotZ.m[1][0] = -sin(rotAngle.z);
	matRotZ.m[1][1] = cos(rotAngle.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
}

void SphereObj::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

void SphereObj::SphereInit(ID3D12Device* dev, Shader shader, const int u_size, const int v_size)
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
#pragma endregion

	// ���_�f�[�^�T�C�Y
	vertexSize = (u_size + 1) * v_size;
	vertices.resize(vertexSize);

	for (int v = 0; v < v_size; v++)
	{
		for (int u = 0; u <= u_size; u++)
		{
			float theta = MyMath::PI * v / (float)(v_size - 1);
			float phi = MyMath::PI * 2 * u / (float)u_size;
			Vector3D pos(sinf(theta) * cosf(phi),
				cosf(theta),
				sinf(theta) * sinf(phi));
			if (u != u_size) {
				vertices[(u_size + 1) * v + u].pos = pos;
			}
			else {
				vertices[(u_size + 1) * v + u].pos = vertices[(u_size + 1) * v].pos;
			}
		}
	}

#pragma region uv�ݒ�
	for (int v = 0; v < v_size; v++)
	{
		for (int u = 0; u <= u_size; u++)
		{
			Vector2D uv;
			uv.x = u / (float)u_size;
			uv.y = v / (float)(v_size - 1);

			vertices[(u_size + 1) * v + u].uv = uv;
		}
	}
#pragma endregion

	//	�C���f�b�N�X�T�C�Y
	indexSize = 6 * u_size * (v_size - 1);
	indices.resize(indexSize);
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);

	Vector3D normal;
	int idx = 0;
	for (int v = 0; v < v_size - 1; v++)
	{
		for (int u = 0; u < u_size; u++)
		{
			indices[idx] = u + v * (u_size + 1);
			idx++;
			indices[idx] = u + 1 + v * (u_size + 1);
			idx++;
			indices[idx] = u + (v + 1) * (u_size + 1);
			idx++;
			//normal = CreatePolygonNormal(vertices[indices[idx - 3]].pos, vertices[indices[idx - 2]].pos, vertices[indices[idx - 1]].pos);
			//vertices[indices[idx - 3]].normal = normal;
			//vertices[indices[idx - 2]].normal = normal;
			//vertices[indices[idx - 1]].normal = normal;

			indices[idx] = u + 1 + v * (u_size + 1);
			idx++;
			indices[idx] = u + 1 + (v + 1) * (u_size + 1);
			idx++;
			indices[idx] = u + (v + 1) * (u_size + 1);
			idx++;
			//normal = CreatePolygonNormal(vertices[indices[idx - 3]].pos, vertices[indices[idx - 2]].pos, vertices[indices[idx - 1]].pos);
			//vertices[indices[idx - 3]].normal = normal;
			//vertices[indices[idx - 2]].normal = normal;
			//vertices[indices[idx - 1]].normal = normal;
		}
	}

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	BuffInitialize(sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�@���x�N�g��
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};

	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);

#pragma region  WorldMatrix�����l
	scale = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle = Vector3D(0.0f, 0.0f, 0.0f);
	trans = Vector3D(0.0f, 0.0f, 0.0f);
#pragma endregion
}

SphereObj::SphereObj()
{
}

SphereObj::SphereObj(ID3D12Device* dev, Shader shader, const int u_size, const int v_size)
{
	SphereInit(dev, shader, u_size, v_size);
}

void SphereObj::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	matWorld.Identity();

	//	�X�P�[�����O
	SetMatScaling();
	matWorld *= matScale;

	//	��]
	SetMatRotation();
	matWorld *= matRot;

	//	���s�ړ�
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void SphereObj::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuffUpdate();
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}
