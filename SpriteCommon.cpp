#include "SpriteCommon.h"
#include "DirectX.h"

void SpriteCommon::Initialize(ID3D12Device* dev)
{
	//pv[0] = { {-1.0f,-1.0f,0.1f},{0,1} };
	//pv[1] = { {-1.0f, 1.0f,0.1f},{0,0} };
	//pv[2] = { { 1.0f,-1.0f,0.1f},{1,1} };
	//pv[3] = { { 1.0f, 1.0f,0.1f},{1,0} };
	//pv[0] = { {0.0f,100.0f,0.0f},{0,1} };
	//pv[1] = { {0.0f, 0.0f,0.0f},{0,0} };
	//pv[2] = { { 100.0f,100.0f,0.0f},{1,1} };
	//pv[3] = { { 100.0f, 0.0f,0.0f},{1,0} };
	//vertexSize = 4;
	//UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	//VBInitialize(dev, sizePV, vertexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyzÀ•W
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uvÀ•W
	};

	shader.Init(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");
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
