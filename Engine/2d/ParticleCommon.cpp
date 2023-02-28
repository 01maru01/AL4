#include "ParticleCommon.h"
#include "DirectX.h"

ParticleCommon* ParticleCommon::GetInstance()
{
	static ParticleCommon* instance = new ParticleCommon;
	return instance;
}

void ParticleCommon::DeleteInstance()
{
	delete ParticleCommon::GetInstance();
}

void ParticleCommon::Initialize()
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyzç¿ïW
	};

	Shader shader;
	shader.Initialize(L"Resources/shader/ParticleVS.hlsl", L"Resources/shader/ParticlePS.hlsl","main", L"Resources/shader/ParticleGS.hlsl");
	pipeline.Init(shader, inputLayout, _countof(inputLayout), 3, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	pipeline.SetBlend(GPipeline::ALPHA_BLEND);
}

void ParticleCommon::Draw()
{
	pipeline.Setting();
	pipeline.Update(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}
