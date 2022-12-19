#include "ObjHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	//float3 lightcolor = float3(1, 1, 1);
	float4 wnormal = normalize(mul(matworld, float4(normal, 0)));
	float4 wpos = mul(matworld, pos);
	//float3 ambient = m_ambient;
	//float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
	////const float3 eye = float3(0, 0, -20);
	//const float shininess = 4.0f;
	//float3 eyedir = normalize(cameraPos - wpos.xyz);
	//float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	VSOutput output;
	output.svpos = mul(mul(matview, matworld), pos);
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;
	return output;
}
