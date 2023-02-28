#include "Particle.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	//float4 texcolor = float4(tex.Sample(smp,input.uv));
	//float3 light = normalize(float3(1,-1,1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3;

	float fogStart = 0.1f;
	float fogEnd = 10.0f;
	float4 fogColor = float4(1.0f, 1.0f, 1.0f, 1.0f);                  //フォグカラー

	const float near = 0.1;
	const float far = 30.0;
	const float linerDepth = 1.0 / (far - near);

	float linerPos = length(cameraPos - input.worldpos) * linerDepth;
	float fogFactor = saturate((fogEnd - linerPos) / (fogEnd - fogStart));

	return lerp(fogColor, float4(tex.Sample(smp, input.uv)), fogFactor);

	//return float4(tex.Sample(smp, input.uv));
}