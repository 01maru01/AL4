#pragma once
#include "Shader.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include "DirectX.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class GPipeline
{
public:
	enum BlendMord
	{
		NONE_BLEND = 0,
		ADD_BLEND,
		SUB_BLEND,
		INV_BLEND,
		ALPHA_BLEND,
	};
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static MyDirectX* dx;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> state;

private:
	void SetShader(Shader shader);
	void Blend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, const int mord = NONE_BLEND);
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescriptorRange, UINT numDescriptorRanges, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetRootSignature(UINT rootParamNum = 1);
public:
	GPipeline() {};
	GPipeline(Shader shader, int mord, int constBuffNum);
	void Init(Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, int constBuffNum, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK, D3D12_DEPTH_WRITE_MASK depth_write_mask = D3D12_DEPTH_WRITE_MASK_ALL, bool isDeep = true);
	void Update(D3D_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void Setting();
	void SetBlend(int mord);
};

