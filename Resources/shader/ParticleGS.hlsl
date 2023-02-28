#include "Particle.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] = {
	float4(-0.5f, -0.5f, 0.0f, 0.0f),
	float4(-0.5f, +0.5f, 0.0f, 0.0f),
	float4(+0.5f, -0.5f, 0.0f, 0.0f),
	float4(+0.5f, +0.5f, 0.0f, 0.0f),
};
static const float4 bottom_offset_array[vnum] = {
	float4(-0.5f, +0.0f, 0.0f, 0.0f),
	float4(-0.5f, +1.0f, 0.0f, 0.0f),
	float4(+0.5f, +0.0f, 0.0f, 0.0f),
	float4(+0.5f, +1.0f, 0.0f, 0.0f),
};

static const float2 uv_array[vnum] = {
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f),
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
		offset.x += windForce * sin(dot(offset.xz, windDir.xz) + elapsedTime) * offset.y;

		offset = mul(matBillboard, offset);
		//offset = mul(matWorld, offset);
		float4 pos_ = input[0].svpos + offset;
		element.worldpos = pos_;
		element.svpos = mul(mat, pos_);
		element.uv = uv_array[i];
		output.Append(element);
	}
}