#include "ObjHeader.hlsli"

SkinOutput ComputeSkin(float4 pos : POSITION, float3 normal : NORMAL, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
{
	SkinOutput output = (SkinOutput)0;

	uint iBone;
	float weight;
	matrix m;

	iBone = boneIndices.x;
	weight = boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, pos);
	output.normal += weight * mul((float3x3)m, normal);

	iBone = boneIndices.y;
	weight = boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, pos);
	output.normal += weight * mul((float3x3)m, normal);

	iBone = boneIndices.z;
	weight = boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, pos);
	output.normal += weight * mul((float3x3)m, normal);

	iBone = boneIndices.w;
	weight = boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, pos);
	output.normal += weight * mul((float3x3)m, normal);

	return output;
}


VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
{
	SkinOutput skinned = ComputeSkin(pos, normal, boneIndices, boneWeights);

	float4 wnormal = normalize(mul(matworld, float4(skinned.normal, 0)));
	VSOutput output;

	output.svpos = mul(mul(matview, matworld), skinned.pos);
	output.worldpos = mul(matworld, skinned.pos);
	output.normal = wnormal;
	output.uv = uv;

	return output;
}