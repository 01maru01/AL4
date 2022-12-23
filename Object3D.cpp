#include "Object3D.h"
#include "ICollider.h"

MyDirectX* Object3D::dx = MyDirectX::GetInstance();
ICamera* Object3D::camera = nullptr;
GPipeline Object3D::pipeline;
Light* Object3D::light = nullptr;

Object3D::~Object3D()
{
	if (collider) {
		delete collider;
	}
	if (model) {
		delete model;
	}
}

Object3D* Object3D::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3D* obj = new Object3D();
	if (obj == nullptr) {
		return nullptr;
	}

	// 初期化
	obj->Initialize();

	if (model) {
		obj->SetModel(model);
	}

	return obj;
}

void Object3D::Initialize()
{
	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = transform->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(result));

	//	リソース設定
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	定数バッファのマッピング
	result = material->Map(0, nullptr, (void**)&constMapMaterial);	//	マッピング
	assert(SUCCEEDED(result));
#pragma endregion
}

void Object3D::Update()
{
#pragma region WorldMatrix
	mat.Update();
#pragma endregion

	constMapTransform->matworld = mat.matWorld;
	constMapTransform->matview = camera->GetMatrix();
	constMapTransform->matview *= camera->GetProjectioin();
	constMapTransform->cameraPos = camera->GetEye();

	if (collider) {
		collider->Update();
	}
}

void Object3D::Draw()
{
	// モデルの割り当てがなければ描画しない
	if (model == nullptr) {
		return;
	}

	// パイプラインステートの設定
	pipeline.Setting(dx->GetCmdList());
	pipeline.Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
	//dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(textureHandle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	// ライトの描画
	light->Draw();

	// モデル描画
	model->Draw();
}

void Object3D::SetCollider(ICollider* collider)
{
	collider->SetObject3D(this);
	this->collider = collider;
}
