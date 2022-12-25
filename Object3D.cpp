#include "Object3D.h"
#include "BaseCollider.h"
#include "CollisionManager.h"

Light* Object3D::light = nullptr;
GPipeline* Object3D::pipeline = nullptr;
ICamera* Object3D::camera = nullptr;
MyDirectX* Object3D::dx = MyDirectX::GetInstance();

void Object3D::SetLight(Light* light)
{
	Object3D::light = light;
}

void Object3D::SetPipeline(GPipeline* pipeline_)
{
	Object3D::pipeline = pipeline_;
}

void Object3D::SetCamera(ICamera* camera)
{
	Object3D::camera = camera;
}

void Object3D::SetModel(Model* model)
{
	this->model = model;
}

void Object3D::SetCollider(BaseCollider* collider)
{
	collider->SetObject3D(this);
	this->collider = collider;
	CollisionManager::GetInstance()->AddCollider(collider);
	collider->Update();
}

Object3D::~Object3D()
{
	if (collider) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

Object3D* Object3D::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Object3D* obj = new Object3D();
	if (obj == nullptr) {
		return nullptr;
	}

	// ������
	obj->Initialize();

	if (model) {
		obj->SetModel(model);
	}

	return obj;
}

void Object3D::Initialize()
{
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

	mat.Initialize();
}

void Object3D::ColliderUpdate()
{
	if (collider) {
		collider->Update();
	}
}

void Object3D::MatUpdate()
{
	HRESULT result;
#pragma region WorldMatrix
	mat.Update();
#pragma endregion

	//if (isBillboard) {
	//	const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

	//	matWorld = XMMatrixIdentity();
	//	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	//	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	//	matWorld *= matBillboard;
	//	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f
	//}

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		mat.matWorld *= parent->mat.matWorld;
	}

	const Matrix& matViewProjection = camera->GetViewProj();
	const Vector3D& cameraPos = camera->GetEye();

	ConstBufferDataTransform* constMap = nullptr;
	result = transform->Map(0, nullptr, (void**)&constMap);
	constMap->matview = matViewProjection;
	constMap->matworld = mat.matWorld;
	constMap->cameraPos = cameraPos;
	transform->Unmap(0, nullptr);
}

void Object3D::Draw()
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	light->Draw();

	model->Draw();
}
