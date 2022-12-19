cbuffer ConstBufferDataTransform:register(b1)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

cbuffer ConstBufferDataMaterial:register(b0)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

cbuffer ConstBufferLightData:register(b2)
{
	float3 lightv;
	float3 lightcolor;
};

struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 svpos : SV_POSITION;	//	���W���
	float4 worldpos:	POSITION;
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
};
