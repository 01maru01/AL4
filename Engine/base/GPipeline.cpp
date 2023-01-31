#include "GPipeline.h"
#include <vector>
#include "DirectX.h"

MyDirectX* GPipeline::dx = MyDirectX::GetInstance();

void GPipeline::Update(D3D_PRIMITIVE_TOPOLOGY primitive)
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	dx->GetCmdList()->SetPipelineState(state.Get());
	dx->GetCmdList()->IASetPrimitiveTopology(primitive);
}

void GPipeline::Setting()
{
	dx->GetCmdList()->SetGraphicsRootSignature(rootSignature.Get());
}

void GPipeline::Init(Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, int constBuffNum, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord, D3D12_DEPTH_WRITE_MASK depth_write_mask, bool isDeep)
{
	HRESULT result;
	// �V�F�[�_�[�̐ݒ�
	SetShader(shader);

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

#pragma region Rasterizer
	// �ݒ�
	pipelineDesc.RasterizerState.CullMode = cullmord; // �w�ʃJ�����O
	pipelineDesc.RasterizerState.FillMode = fillmord; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
#pragma endregion

#pragma region  Blending
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc);
#pragma endregion

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = inputLayoutSize;

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = topologyType;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;							  // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;							  // 1�s�N�Z���ɂ�1��T���v�����O

	//	�f�v�X�X�e���V���X�e�[�g�ݒ�
	if (isDeep) {
		pipelineDesc.DepthStencilState.DepthEnable = true;								//	�[�x�e�X�g���s��
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//	��������΍��i
	}
	else {
		pipelineDesc.DepthStencilState.DepthEnable = false;
		pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	}
	pipelineDesc.DepthStencilState.DepthWriteMask = depth_write_mask;		//	�������݋����邩�ǂ���
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;							//	�[�x�t�H�[�}�b�g

	SetRootSignature(constBuffNum);

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v�����X�e�[�g�̐���
	result = dx->GetDev()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&state));
	assert(SUCCEEDED(result));
}

void GPipeline::Blend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, const int mord)
{
	//	���ʐݒ�
	if (mord != NONE_BLEND) {
		pipelineDesc.BlendState.AlphaToCoverageEnable = true;
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	}
	else {
		pipelineDesc.BlendState.AlphaToCoverageEnable = false;
	}

	switch (mord)
	{
	case ADD_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case SUB_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case INV_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case ALPHA_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}
}

void GPipeline::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;							//	�萔�o�b�t�@�r���[
	rootParam.Descriptor.ShaderRegister = shaderRegister;	//	�萔�o�b�t�@�ԍ�
	rootParam.Descriptor.RegisterSpace = registerSpace;		//	�f�t�H���g�l
	rootParam.ShaderVisibility = shaderVisibility;			//	���ׂẴV�F�[�_���猩��
}

void GPipeline::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescriptorRange, UINT numDescriptorRanges, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.DescriptorTable.pDescriptorRanges = &pDescriptorRange;
	rootParam.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
	rootParam.ShaderVisibility = shaderVisibility;
}

void GPipeline::SetRootSignature(UINT rootParamNum)
{
	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma region	���[�g�p�����[�^
	//	���[�g�p�����[�^�̐ݒ�
	std::vector<D3D12_ROOT_PARAMETER> rootParams = {};
	rootParams.resize(rootParamNum + 1);
	SetRootParam(rootParams[0], D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, descriptorRange, 1);
	for (size_t i = 0; i < rootParamNum; i++)
	{
		SetRootParam(rootParams[i + 1], D3D12_ROOT_PARAMETER_TYPE_CBV, (UINT)i, 0);
	}
#pragma endregion

#pragma region sampler
	//	�e�N�X�`���[�T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
#pragma endregion

#pragma region ���[�g�V�O�l�`��
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();						//	�擪�A�h���X
	rootSignatureDesc.NumParameters = (UINT)rootParams.size();						//	���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		rootSigBlob.ReleaseAndGetAddressOf(),
		errorBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	result = dx->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
#pragma endregion
}

GPipeline::GPipeline(Shader shader, int mord, int constBuffNum)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�@���x�N�g��
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};

	Init(shader, inputLayout, _countof(inputLayout), constBuffNum);
	SetBlend(mord);
}

void GPipeline::SetBlend(int mord)
{
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc, mord);
	HRESULT result = dx->GetDev()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&state));
	assert(SUCCEEDED(result));
}

void GPipeline::SetShader(Shader shader)
{
#pragma region VertexShader
	pipelineDesc.VS.pShaderBytecode = shader.VSBlob()->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader.VSBlob()->GetBufferSize();
#pragma endregion
#pragma region HShader
	if (shader.HSBlob() != nullptr) {
		pipelineDesc.HS.pShaderBytecode = shader.HSBlob()->GetBufferPointer();
		pipelineDesc.HS.BytecodeLength = shader.HSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region DShader
	if (shader.DSBlob() != nullptr) {
		pipelineDesc.DS.pShaderBytecode = shader.DSBlob()->GetBufferPointer();
		pipelineDesc.DS.BytecodeLength = shader.DSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region GShader
	if (shader.GSBlob() != nullptr) {
		pipelineDesc.GS.pShaderBytecode = shader.GSBlob()->GetBufferPointer();
		pipelineDesc.GS.BytecodeLength = shader.GSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region PixcelShader
	pipelineDesc.PS.pShaderBytecode = shader.PSBlob()->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader.PSBlob()->GetBufferSize();
#pragma endregion
}
