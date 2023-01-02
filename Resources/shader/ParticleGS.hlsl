#include "Particle.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] = {
	float4(-0.5f, -0.5f, 0, 0),
	float4(-0.5f, +0.5f, 0, 0),
	float4(+0.5f, -0.5f, 0, 0),
	float4(+0.5f, +0.5f, 0, 0),
};
static const float4 bottom_offset_array[vnum] = {
	float4(-0.5f, +0.0f, 0, 0),
	float4(-0.5f, +1.0f, 0, 0),
	float4(+0.5f, +0.0f, 0, 0),
	float4(+0.5f, +1.0f, 0, 0),
};

static const float2 uv_array[vnum] = {
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0),
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	for (uint i = 0; i < vnum; i++) {
		float4 offset = bottom_offset_array[i] * scale;
		offset = mul(matBillboard, offset);
		//offset = mul(matWorld, offset);
		element.svpos = input[0].svpos + offset;
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}