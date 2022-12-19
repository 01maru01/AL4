#include "ObjHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float3 shader_color = m_ambient;
	//shader_color += m_diffuse * diffuse;
	float3 lightcolor = float3(1, 1, 1);
	float4 shadercolor;
	const float shininess = 4.0f;

	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float3 dolightnormal = dot(lightv, input.normal);
	float3 reflect = normalize(-lightv + 2 * dolightnormal * input.normal);

	float3 ambient = m_ambient;
	float3 diffuse = dolightnormal * m_diffuse;
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	shadercolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadercolor.a = m_alpha;

	float4 texcolor = float4(tex.Sample(smp,input.uv));
	return shadercolor * texcolor;
	//return float4(texcolor.rgb * shader_color, texcolor.a*m_alpha);
}