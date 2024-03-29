#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;


Texture2D		g_DissolveTexture;
 

float3          g_vBloomPower;
float4			g_vCamPosition;

cbuffer InversTransposeMatBuffer
{
    matrix WorldInvTransposeView;
};

float g_fMotionTrailAlpha;

struct KeyframeDesc
{
	int iAnimIndex;
	uint iCurFrame;
	uint iNextFrame;
	float fRatio;
	float fFrameAcc;

	uint	iFinish;
	uint	iFix;
	uint	iStop;
};

struct TweenFrameDesc
{
	KeyframeDesc cur;
	KeyframeDesc next;

	float fTweenDuration;
	float fTweenRatio;
	float fTweenAcc;
	float fPadding;
};

TweenFrameDesc  g_TweenFrames;
TweenFrameDesc  g_TweenFrames_Array[200];
Texture2DArray  g_TransformMap;

struct AnimInstancingDesc
{
    float4 vRimColor;
    float4 vDissolveColor;
	
    float fDissolveTotal;
    float fDissolveDuration;
    float fDissolveWeight;
    float fDissolveSpeed;
};
AnimInstancingDesc g_AnimInstancingDesc[200];

struct VS_IN
{
	float3		vPosition		: POSITION;
	float3		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	uint4		vBlendIndex		: BLENDINDEX;
	float4		vBlendWeight	: BLENDWEIGHT;

	float4		vRight			: TEXCOORD1;
	float4		vUp				: TEXCOORD2;
	float4		vLook			: TEXCOORD3;
	float4		vTranslation	: TEXCOORD4;

	uint		iInstanceID		: SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition		: SV_POSITION0;
	float4		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	float3		vBinormal		: BINORMAL;
	float4		vProjPos		: TEXCOORD1;
	float4		vWorldPosition	: TEXCOORD2;

	uint		iInstanceID		: SV_INSTANCEID;
	
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

#define QUATERNION_IDENTITY float4(0, 0, 0, 1)
float4 QSlerp(in float4 a, in float4 b, float t)
{
	// if either input is zero, return the other.
	if (length(a) == 0.0)
	{
		if (length(b) == 0.0)
		{
			return QUATERNION_IDENTITY;
		}
		return b;
	}
	else if (length(b) == 0.0)
	{
		return a;
	}

	float cosHalfAngle = a.w * b.w + dot(a.xyz, b.xyz);

	if (cosHalfAngle >= 1.0 || cosHalfAngle <= -1.0)
	{
		return a;
	}
	else if (cosHalfAngle < 0.0)
	{
		b.xyz = -b.xyz;
		b.w = -b.w;
		cosHalfAngle = -cosHalfAngle;
	}

	float blendA;
	float blendB;
	if (cosHalfAngle < 0.99)
	{
		// do proper slerp for big angles
		float halfAngle = acos(cosHalfAngle);
		float sinHalfAngle = sin(halfAngle);
		float oneOverSinHalfAngle = 1.0 / sinHalfAngle;
		blendA = sin(halfAngle * (1.0 - t)) * oneOverSinHalfAngle;
		blendB = sin(halfAngle * t) * oneOverSinHalfAngle;
	}
	else
	{
		// do lerp if angle is really small.
		blendA = 1.0 - t;
		blendB = t;
	}

	float4 result = float4(blendA * a.xyz + blendB * b.xyz, blendA * a.w + blendB * b.w);
	if (length(result) > 0.0)
	{
		return normalize(result);
	}
	return QUATERNION_IDENTITY;
}

matrix GetAnimationMatrix(VS_IN input)
{
	float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
	float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2];

	/* cur */
	animIndex[0]	= g_TweenFrames_Array[input.iInstanceID].cur.iAnimIndex;
	currFrame[0]	= g_TweenFrames_Array[input.iInstanceID].cur.iCurFrame;
	nextFrame[0]	= g_TweenFrames_Array[input.iInstanceID].cur.iNextFrame;
	ratio[0]		= g_TweenFrames_Array[input.iInstanceID].cur.fRatio;

	/* next */
	animIndex[1]	= g_TweenFrames_Array[input.iInstanceID].next.iAnimIndex;
	currFrame[1]	= g_TweenFrames_Array[input.iInstanceID].next.iCurFrame;
	nextFrame[1]	= g_TweenFrames_Array[input.iInstanceID].next.iNextFrame;
	ratio[1]		= g_TweenFrames_Array[input.iInstanceID].next.fRatio;

	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;

	for (int i = 0; i < 4; i++)
	{
		/* cur */
		c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
		c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
		c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
		c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		curr = matrix(c0, c1, c2, c3);

		n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
		n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
		n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
		n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		next = matrix(n0, n1, n2, n3);

		// matrix result = matrix(QSlerp(c0, n0, ratio[0]), QSlerp(c1, n1, ratio[0]), QSlerp(c2, n2, ratio[0]), lerp(c3, n3, ratio[0]));
		matrix result = lerp(curr, next, ratio[0]);

		/* if next */
		if (animIndex[1] >= 0)
		{
			c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
			c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
			c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
			c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			curr = matrix(c0, c1, c2, c3);

			n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
			n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
			n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
			n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
			next = matrix(n0, n1, n2, n3);

			matrix nextResult = lerp(curr, next, ratio[1]);
			result = lerp(result, nextResult, g_TweenFrames_Array[input.iInstanceID].fTweenRatio);
		}

		transform += mul(weights[i], result);
	}

	return transform;
}

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix BoneMatrix	= GetAnimationMatrix(In);
	vector vPosition	= mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

	float4x4 InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	matrix matWV	= mul(InstanceWorld, g_ViewMatrix);
	matrix matWVP	= mul(matWV, g_ProjMatrix);

	

	Out.vPosition		= mul(vPosition, matWVP);
	Out.vWorldPosition	= mul(vPosition, InstanceWorld);
	Out.vNormal			= normalize(mul(vNormal, InstanceWorld));
	Out.vTangent		= normalize(mul(vTangent, InstanceWorld)).xyz;
	Out.vBinormal		= normalize(cross(Out.vNormal, Out.vTangent));
	Out.vTexUV			= In.vTexUV;
	Out.vProjPos		= Out.vPosition;
	Out.iInstanceID		= In.iInstanceID;

    Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
    Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);
	
	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vProjPos : TEXCOORD1;
	float4		vWorldPosition : TEXCOORD2;
	
    uint		iInstanceID : SV_INSTANCEID;
	
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
    float4      vBloom : SV_TARGET3;
    float4		vViewNormal : SV_TARGET4;
};

float4 Caculation_Brightness(float4 vColor)
{
    float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

    float fPixelBrightness = dot(vColor.rgb, g_vBloomPower.rgb);
    if (fPixelBrightness > 0.99f)
        vBrightnessColor = float4(vColor.rgb, 1.0f);

    return vBrightnessColor;
}

SamplerState MirrorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Mirror;
    AddressV = Mirror;
    AddressW = Mirror;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_AnimInstancingDesc[In.iInstanceID].fDissolveWeight / g_AnimInstancingDesc[In.iInstanceID].fDissolveDuration + vDissoveTexture.r);
	
	// �ȼ� ����
    if (fDissolveAlpha < 0.3)
        discard;
	
	// �ȼ� ���� ���� : ���� ���� X
    else if (fDissolveAlpha < 0.5)
    {
        Out.vDiffuse = g_AnimInstancingDesc[In.iInstanceID].vDissolveColor;
        Out.vNormal  = float4(1.f, 1.f, 1.f, 0.f);
        Out.vBloom   = float4(Out.vDiffuse.r, Out.vDiffuse.g, Out.vDiffuse.b, 0.5f);
    }
	
	// �⺻ �ȼ� ������
    else
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
        Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);


	float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_AnimInstancingDesc[In.iInstanceID].vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;

	if (0.f == Out.vDiffuse.a)
		discard;

        if (0.f == Out.vDiffuse.a)
            discard;
    }
	
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return Out;
}

PS_OUT PS_WITCH_WOOD(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	// �⺻ �ȼ� ������
  
    Out.vDiffuse = g_DiffuseTexture.Sample(MirrorSampler, In.vTexUV);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_AnimInstancingDesc[In.iInstanceID].vRimColor * fRimPower;
    Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;

    if (0.f == Out.vDiffuse.a)
        discard;

    
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Out;
}

PS_OUT PS_GRANDFA_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_AnimInstancingDesc[In.iInstanceID].fDissolveWeight / g_AnimInstancingDesc[In.iInstanceID].fDissolveDuration + vDissoveTexture.r);
	
	// �ȼ� ����
    if (fDissolveAlpha < 0.3)
        discard;
	
	// �ȼ� ���� ���� : ���� ���� X
    else if (fDissolveAlpha < 0.5)
    {
        Out.vDiffuse = g_AnimInstancingDesc[In.iInstanceID].vDissolveColor;
        Out.vNormal = float4(1.f, 1.f, 1.f, 0.f);
        Out.vBloom = float4(Out.vDiffuse.r, Out.vDiffuse.g, Out.vDiffuse.b, 0.5f);
    }
	
	// �⺻ �ȼ� ������
    else
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
        Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);


        float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
        fRimPower = pow(fRimPower, 5.f);
        vector vRimColor = g_AnimInstancingDesc[In.iInstanceID].vRimColor * fRimPower;
        Out.vDiffuse += vRimColor;
        Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;

        if (0.5f >= Out.vDiffuse.a)
            discard;
    }
	
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Out;
}

PS_OUT PS_MAIN_NORMAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_AnimInstancingDesc[In.iInstanceID].fDissolveWeight / g_AnimInstancingDesc[In.iInstanceID].fDissolveDuration + vDissoveTexture.r);
	
	// �ȼ� ����
    if (fDissolveAlpha < 0.3)
        discard;
	
	// �ȼ� ���� ���� : ���� ���� X
    else if (fDissolveAlpha < 0.5)
    {
        Out.vDiffuse = g_AnimInstancingDesc[In.iInstanceID].vDissolveColor;
        Out.vNormal = float4(1.f, 1.f, 1.f, 0.f);
        Out.vBloom = float4(Out.vDiffuse.r, Out.vDiffuse.g, Out.vDiffuse.b, 0.5f);
    }
	
	// �⺻ �ȼ� ������
    else
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);

        vector vTextureNormal = g_NormalTexture.Sample(ModelSampler, In.vTexUV);

        float3 vNormal = vTextureNormal.xyz * 2.f - 1.f;
        float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

        vNormal = normalize(mul(vNormal, WorldMatrix));

        Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);


    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 5.f);
    vector vRimColor = g_AnimInstancingDesc[In.iInstanceID].vRimColor * fRimPower;
	
	Out.vDiffuse += vRimColor;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse) + vRimColor;


        if (0.f == Out.vDiffuse.a)
            discard;
    }

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return Out;
}

PS_OUT PS_DISSOLVE_DEAD(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

    vector vDissoveTexture = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = saturate(1.0 - g_AnimInstancingDesc[In.iInstanceID].fDissolveWeight / g_AnimInstancingDesc[In.iInstanceID].fDissolveDuration + vDissoveTexture.r);

	
	// �ȼ� ����
    if (fDissolveAlpha < 0.3)
        discard;
	
	// �ȼ� ���� ���� : ���� ���� X
    else if (fDissolveAlpha < 0.5)
    {
        Out.vDiffuse = g_AnimInstancingDesc[In.iInstanceID].vDissolveColor;
        Out.vNormal = float4(1.f, 1.f, 1.f, 0.f);
        Out.vBloom = float4(Out.vDiffuse.r, Out.vDiffuse.g, Out.vDiffuse.b, 0.5f);
    }
	// �⺻ �ȼ� ������
    else
    {
        Out.vDiffuse = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);
        Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
        Out.vBloom = Caculation_Brightness(Out.vDiffuse);
    }
    
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (0.f == Out.vDiffuse.a)
        discard;

	return Out;
}

PS_OUT PS_MOTION_TRAIL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 1.0f, 0.0f);

    float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
    fRimPower = pow(fRimPower, 20.f);
	
    vector vRimColor = g_AnimInstancingDesc[In.iInstanceID].vRimColor * fRimPower;
    Out.vDiffuse = vRimColor;
    Out.vDiffuse.a = 1.f;
    Out.vBloom = Caculation_Brightness(Out.vDiffuse);
    Out.vViewNormal = float4(1.0f, 1.0f, 1.0f, 1.0f);
    return Out;
}

// �׸��� �ȼ� ���̴�.
struct PS_OUT_SHADOW_DEPTH
{
	float4		vDepth : SV_TARGET0;
};


PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
	PS_OUT_SHADOW_DEPTH Out = (PS_OUT_SHADOW_DEPTH)0;

	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (vColor.a <= 0.01f)
		discard;


	Out.vDepth = vector(In.vProjPos.w / 1000.0f, In.vProjPos.w / 1000.0f, 0.f, 1.f);

	return Out;
}


technique11 DefaultTechnique
{
	pass DefaultPass
	{
		// 0
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalPass
	{
		// 1
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass DissolveDead
	{
		// 2
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE_DEAD();
	}

	pass MotionTrail
	{
		// 3
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MOTION_TRAIL();
	}

    pass WoodNpc
    {
		// 4
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_GRANDFA_MAIN();
    }

    pass WitchWood
    {
		// 5
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_WITCH_WOOD();
    }

	pass Temp3
	{
		// 6
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp4
	{
		// 6
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp5
	{
		// 7
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp6
	{
		// 8
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Shadow_Depth
	{
		// 10
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Shadow, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_DEPTH();
	}
};