#include "SpriteCommon.h"
#include "DirectX.h"

SpriteCommon::SpriteCommon()
{
	Initialize();
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon* instance = new SpriteCommon;
	return instance;
}

void SpriteCommon::DeleteInstance()
{
	delete SpriteCommon::GetInstance();
}

void SpriteCommon::Initialize()
{
	ID3D12Device* dev = MyDirectX::GetInstance()->GetDev();
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyzÀ•W
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uvÀ•W
	};

	shader.Initialize(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");
	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, false);
	pipeline.SetBlend(dev, GPipeline::ALPHA_BLEND);


	mat2D.Identity();
	mat2D.m[0][0] = 2.0f / Window::window_width;
	mat2D.m[1][1] = -2.0f / Window::window_height;
	mat2D.m[3][0] = -1;
	mat2D.m[3][1] = 1;
}

void SpriteCommon::Draw()
{
	pipeline.Setting(MyDirectX::GetInstance()->GetCmdList());
	pipeline.Update(MyDirectX::GetInstance()->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
