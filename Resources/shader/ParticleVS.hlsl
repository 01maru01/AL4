#include "ParticleHeader.hlsli"

VSOutput main(float4 pos : POSITION, float scale : TEXCOORD)
{
	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = pos;
	output.scale = scale;
	//output.normal = normal;
	//output.uv = uv;
	return output;
}