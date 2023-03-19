static const int POINTLIGHT_NUM = 3;
static const int DIRLIGHT_NUM = 3;
static const int SPOTLIGHT_NUM = 3;
static const int CIRCLESHADOW_NUM = 1;

static const int MAX_BONES = 32;

cbuffer skinning:register(b3)
{
	matrix matSkinning[MAX_BONES];
}

struct PointLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

struct DirLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

struct SpotLight
{
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightfactoranglecos;
	uint active;
};

struct CircleShadow
{
	float3 dir;
	float3 casterPos;
	float distanceCasterLight;
	float3 atten;
	float2 factorAngleCos;
	uint active;
};

cbuffer ConstBufferDataMaterial:register(b0)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

cbuffer ConstBufferDataTransform:register(b1)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

cbuffer ConstBufferLightData:register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
};

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
	float4 worldpos:	POSITION;
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
};

struct SkinOutput
{
	float4 pos;
	float3 normal;
};