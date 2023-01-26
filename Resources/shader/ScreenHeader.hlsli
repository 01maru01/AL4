cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;	//	RGBA
};

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
	float2 uv :TEXCOORD;
};