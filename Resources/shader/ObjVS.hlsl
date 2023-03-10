#include "ObjHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
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
	VSOutput output = (VSOutput)0;
	output.svpos = mul(mul(matview, matworld), pos);
	//output.worldpos = wpos;
	//output.normal = wnormal.xyz;
	output.uv = uv;

	uint iBone;
	float weight;
	matrix m;

	iBone = boneIndices.x;
	weight = boneWeights.x;
	m = matSkinning[iBone];
	output.worldpos += weight * mul(m, wpos);
	output.normal += weight * mul((float3x3)m, wnormal.xyz);

	iBone = boneIndices.y;
	weight = boneWeights.y;
	m = matSkinning[iBone];
	output.worldpos += weight * mul(m, wpos);
	output.normal += weight * mul((float3x3)m, wnormal.xyz);

	iBone = boneIndices.z;
	weight = boneWeights.z;
	m = matSkinning[iBone];
	output.worldpos += weight * mul(m, wpos);
	output.normal += weight * mul((float3x3)m, wnormal.xyz);

	iBone = boneIndices.w;
	weight = boneWeights.w;
	m = matSkinning[iBone];
	output.worldpos += weight * mul(m, wpos);
	output.normal += weight * mul((float3x3)m, wnormal.xyz);

	return output;
}
