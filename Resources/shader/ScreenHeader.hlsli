cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;	//	RGBA
};

struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 svpos : SV_POSITION;	//	���W���
	float2 uv :TEXCOORD;
};