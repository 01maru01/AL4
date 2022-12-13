#include "ObjHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float3 shader_color = m_ambient;
	//shader_color += m_diffuse * diffuse;
	float4 texcolor = float4(tex.Sample(smp,input.uv));
	return input.color;
	//return float4(texcolor.rgb * shader_color, texcolor.a*m_alpha);
}