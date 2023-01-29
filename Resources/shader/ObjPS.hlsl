#include "ObjHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float3 shader_color = m_ambient;
	//shader_color += m_diffuse * diffuse;
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	const float shininess = 4.0f;
	float4 shadercolor;

	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		float3 lightcolor = float3(1, 1, 1);

		float3 dolightnormal = dot(dirLights[i].lightv, input.normal);
		float3 reflect = normalize(-dirLights[i].lightv + 2 * dolightnormal * input.normal);

		float3 ambient = m_ambient;
		float3 diffuse = dolightnormal * m_diffuse;
		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

		shadercolor.rgb = (ambient + diffuse + specular) * dirLights[i].lightcolor;
		shadercolor.a = m_alpha;
	}

	for (i = 0; i < POINTLIGHT_NUM; i++) {
		if (pointLights[i].active) {
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			float3 dotlightnormal = dot(lightv, input.normal);
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadercolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}

	float4 texcolor = float4(tex.Sample(smp,input.uv));
	return shadercolor * texcolor;
	//return float4(texcolor.rgb * shader_color, texcolor.a*m_alpha);
}