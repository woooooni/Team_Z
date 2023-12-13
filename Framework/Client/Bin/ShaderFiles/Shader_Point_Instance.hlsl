
#include "Engine_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture[100];
Texture2D	    g_AlphaTexture;

vector			g_vCamPosition;

struct EffectDesc //16 ����� ��������������
{
	float  g_fTimeAccs;
	float  g_fLifeTime;

	float  g_fSpeed;
	float  g_fAnimationSpeed;

	float  g_fTextureIndex;
	float  g_fTemp1;
	float  g_fTemp2;
	float  g_fTemp3;

	float4 g_fVelocity;
	float4 g_fColor;
};
EffectDesc g_EffectDesc[1000];

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize    : PSIZE;

	float4		vRight : WORLD0;
	float4		vUp    : WORLD1;
	float4		vLook  : WORLD2;
	float4		vTranslation : WORLD3;

	uint	    iInstanceID : SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;

	uint	    iInstanceID : SV_INSTANCEID;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	

	matrix		TransformMatrix = float4x4(normalize(In.vRight), normalize(In.vUp), normalize(In.vLook), In.vTranslation);	

	vector		vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix);
	Out.vPSize    = float2(In.vPSize.x * length(In.vRight), In.vPSize.y * length(In.vUp));

	Out.iInstanceID = In.iInstanceID;

	return Out;	
}

struct GS_IN
{	
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;

	uint	iInstanceID : SV_INSTANCEID;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vProjPos  : TEXCOORD1;

	uint	iInstanceID : SV_INSTANCEID;
};

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];


	/* �޾ƿ� ������ ���������Ͽ� �簢���� �����ϱ����� ���� �������� ����ž�. */
	vector		vLook = g_vCamPosition - In[0].vPosition;

	float3		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
	float3		vUp = normalize(cross(vLook.xyz, vRight.xyz)) * In[0].vPSize.y * 0.5f;

	matrix		matVP;

	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexcoord = float2(0.0f, 0.f);
	Out[0].vProjPos  = Out[0].vPosition;

	Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexcoord = float2(1.0f, 0.f);
	Out[1].vProjPos = Out[1].vPosition;

	Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexcoord = float2(1.0f, 1.0f);
	Out[2].vProjPos = Out[2].vPosition;

	Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexcoord = float2(0.0f, 1.0f);
	Out[3].vProjPos = Out[3].vPosition;

	Out[0].iInstanceID = In[0].iInstanceID;
	Out[1].iInstanceID = In[0].iInstanceID;
	Out[2].iInstanceID = In[0].iInstanceID;
	Out[3].iInstanceID = In[0].iInstanceID;

	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float4		vProjPos  : TEXCOORD1;

	uint	iInstanceID : SV_INSTANCEID;
};

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0;
	float4	vBlurPower : SV_TARGET1;
	float4	vBrightness : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	int iTextureIndex = (int)g_EffectDesc[In.iInstanceID].g_fTextureIndex;
	switch (iTextureIndex)
	{
	case 0:
		Out.vDiffuse = g_DiffuseTexture[0].Sample(PointSampler, In.vTexcoord);
		break;
	case 1:
		Out.vDiffuse = g_DiffuseTexture[1].Sample(PointSampler, In.vTexcoord);
		break;
	case 2:
		Out.vDiffuse = g_DiffuseTexture[2].Sample(PointSampler, In.vTexcoord);
		break;
	case 3:
		Out.vDiffuse = g_DiffuseTexture[3].Sample(PointSampler, In.vTexcoord);
		break;
	case 4:
		Out.vDiffuse = g_DiffuseTexture[4].Sample(PointSampler, In.vTexcoord);
		break;
	case 5:
		Out.vDiffuse = g_DiffuseTexture[5].Sample(PointSampler, In.vTexcoord);
		break;
	case 6:
		Out.vDiffuse = g_DiffuseTexture[6].Sample(PointSampler, In.vTexcoord);
		break;
	case 7:
		Out.vDiffuse = g_DiffuseTexture[7].Sample(PointSampler, In.vTexcoord);
		break;
	case 8:
		Out.vDiffuse = g_DiffuseTexture[8].Sample(PointSampler, In.vTexcoord);
		break;
	case 9:
		Out.vDiffuse = g_DiffuseTexture[9].Sample(PointSampler, In.vTexcoord);
		break;
	default:
		Out.vDiffuse = g_DiffuseTexture[10].Sample(PointSampler, In.vTexcoord);
		break;
	}
	if (Out.vDiffuse.a < 0.5f)
		discard;

	Out.vDiffuse    = (Out.vDiffuse + g_EffectDesc[In.iInstanceID].g_fColor) / 2.f;
	Out.vBlurPower  = float4(1.f, 1.f, 1.f, 0.f);
	Out.vBrightness = Out.vDiffuse;

	return Out;
}

technique11 DefaultTechnique
{
	pass Particle
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}



