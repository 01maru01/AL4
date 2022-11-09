cbuffer ConstBufferDataTransform:register(b0)
{
	matrix mat;
};

cbuffer ConstBufferDataMaterial:register(b1)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 svpos : SV_POSITION;	//	���W���
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
};
