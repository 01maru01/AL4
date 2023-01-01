cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;	//	RGBA
};

cbuffer ConstBufferDataTransform:register(b1)
{
	matrix mat;
	matrix matBillboard;
	matrix matWorld;
};

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
};

struct GSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	//float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
};
