#include "ObjHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);


float4 main(VSOutput input) : SV_TARGET
{
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	const float shininess = 4.0f;
	float4 shadercolor;
	shadercolor.a = m_alpha;
	//	方向ライト
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
	//	点光源
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
	//	スポットライト
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
	//	丸影
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

	////	フォグ
	//float4 m_FogColor = float4(1.0f, 1.0f, 1.0f, 1.0f);                  //フォグカラー
	//float  m_Near = 0.0f;             //フォグの開始位置
	//float  m_Far = 1.0f;             //フォグの終了位置
	//float  m_FogLen = 1.0f;             //m_Far - m_Nearの結果
	////	頂点と視点との距離を計算する
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
	//// //オブジェクトのランバート拡散照明の計算結果とフォグカラーを線形合成する
	//// Out = In.Col * tex2D( tex0, In.Tex ) * ( 1.0f - d ) + m_FogColor * d;
	//
	//float f = (m_Far - d) / (m_Far - m_Near);
	//f = clamp(f, 0.0f, 1.0f);
	////オブジェクトのランバート拡散照明の計算結果とフォグカラーを線形合成する

	float4 texcolor = float4(tex.Sample(smp, input.uv));

	if (distanceFog.active) {
		float4 fogColor = float4(distanceFog.fogColor, 1.0f);		//フォグカラー
		const float linerDepth = 1.0 / (distanceFog.fogFar - distanceFog.fogNear);
		float linerPos = length(cameraPos - input.worldpos) * linerDepth;
		float fogFactor = saturate((distanceFog.fogEnd - linerPos) / (distanceFog.fogEnd - distanceFog.fogStart));

		return lerp(fogColor, shadercolor * texcolor, fogFactor);
	}

	return shadercolor * texcolor;
}