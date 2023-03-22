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
	shadercolor.a = m_alpha;

	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			float3 dolightnormal = dot(dirLights[i].lightv, input.normal);
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dolightnormal * input.normal);

			float3 ambient = m_ambient;
			float3 diffuse = dolightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadercolor.rgb += (ambient + diffuse + specular) * dirLights[i].lightcolor;
		}
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

	for (i = 0; i < SPOTLIGHT_NUM; i++) {
		if (spotLights[i].active) {
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			float atten = 1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d);
			float cos = dot(lightv, spotLights[i].lightv);
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			atten *= angleatten;

			float3 dotlightnormal = dot(lightv, input.normal);
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diffuse = dotlightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadercolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}

	for (i = 0; i < CIRCLESHADOW_NUM; i++) {
		if (circleShadows[i].active) {
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			float d = dot(casterv, circleShadows[i].dir);

			float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			atten *= step(0, d);

			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			float3 lightv = normalize(lightpos - input.worldpos.xyz);

			float cos = dot(lightv, circleShadows[i].dir);
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			atten *= angleatten;

			shadercolor.rgb -= atten;
		}
	}

	////	�t�H�O
	//float4 m_FogColor = float4(1.0f, 1.0f, 1.0f, 1.0f);                  //�t�H�O�J���[
	//float  m_Near = 0.0f;             //�t�H�O�̊J�n�ʒu
	//float  m_Far = 1.0f;             //�t�H�O�̏I���ʒu
	//float  m_FogLen = 1.0f;             //m_Far - m_Near�̌���
	////	���_�Ǝ��_�Ƃ̋������v�Z����
	//float d = distance(input.svpos.xyz, cameraPos.xyz);
	//
	//
	//if( d < m_Near )
	//  d = 0.0f;
	//else if( d > m_Far )
	//  d = 1.0f;
	//else
	//  d = ( d - m_Near ) / ( m_FogLen );
	//
	//// //�I�u�W�F�N�g�̃����o�[�g�g�U�Ɩ��̌v�Z���ʂƃt�H�O�J���[����`��������
	//// Out = In.Col * tex2D( tex0, In.Tex ) * ( 1.0f - d ) + m_FogColor * d;
	//
	//float f = (m_Far - d) / (m_Far - m_Near);
	//f = clamp(f, 0.0f, 1.0f);
	////�I�u�W�F�N�g�̃����o�[�g�g�U�Ɩ��̌v�Z���ʂƃt�H�O�J���[����`��������

	float fogStart = 0.1f;
	float fogEnd = 5.0f;
	float4 fogColor = float4(1.0f, 1.0f, 1.0f, 1.0f);                  //�t�H�O�J���[

	const float near = 0.1;
	const float far = 30.0;
	const float linerDepth = 1.0 / (far - near);

	float linerPos = length(cameraPos - input.worldpos) * linerDepth;
	float fogFactor = saturate((fogEnd - linerPos) / (fogEnd - fogStart));

	float4 texcolor = float4(tex.Sample(smp,input.uv));
	return lerp(fogColor, shadercolor * texcolor, fogFactor);

	//float3 light = normalize(float3(1, -1, 1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse * 0.3f;
	//float4 shadercolor = float4(brightness, brightness, brightness, 1.0f);
	//float4 texcolor = tex.Sample(smp,input.uv);
	//return shadercolor* texcolor;
}