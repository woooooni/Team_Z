#include "Engine_Shader_Defines.hpp"

#include "Matrix.hlsl"
#include "Quaternion.hlsl"
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;

Texture2D		g_DissolveTexture;
float			g_fDissolveWeight;

float4			g_vDissolveColor = { 0.6f, 0.039f, 0.039f, 1.f };
float4			g_vRimColor = { 0.f, 0.f, 0.f, 0.f };
float4			g_vCamPosition;


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
Texture2DArray  g_TransformMap;


struct VS_IN
{
	float3		vPosition		: POSITION;
	float3		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	uint4		vBlendIndex		: BLENDINDEX;
	float4		vBlendWeight	: BLENDWEIGHT;
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
};


// 큐빅 스플라인 보간을 수행하는 함수
float4x4 CubicSplineInterpolation(float4x4 p0, float4x4 p1, float4x4 p2, float4x4 p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    float4x4 interpolatedMatrix;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            float c0 = -0.5f * p0[i][j] + 1.5f * p1[i][j] - 1.5f * p2[i][j] + 0.5f * p3[i][j];
            float c1 = p0[i][j] - 2.5f * p1[i][j] + 2.0f * p2[i][j] - 0.5f * p3[i][j];
            float c2 = -0.5f * p0[i][j] + 0.5f * p2[i][j];
            float c3 = p1[i][j];

            interpolatedMatrix[i][j] = c0 * t3 + c1 * t2 + c2 * t + c3;
        }
    }

    return interpolatedMatrix;
}

// 두 개의 행렬을 큐빅 스플라인으로 보간하는 함수
float4x4 InterpolateMatricesWithCubicSpline(float4x4 matrix1, float4x4 matrix2, float t)
{
    // 추가적인 행렬을 계산
    float4x4 extraMatrix1 = matrix1 * 10.0f - matrix2;
    float4x4 extraMatrix2 = matrix2 * 10.0f - matrix1;

    // 큐빅 스플라인 보간 수행
    float4x4 resultMatrix = CubicSplineInterpolation(extraMatrix1, matrix1, matrix2, extraMatrix2, t);

    return resultMatrix;
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
	animIndex[0] = g_TweenFrames.cur.iAnimIndex;
	currFrame[0] = g_TweenFrames.cur.iCurFrame;
	nextFrame[0] = g_TweenFrames.cur.iNextFrame;
	ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
	animIndex[1] = g_TweenFrames.next.iAnimIndex;
	currFrame[1] = g_TweenFrames.next.iCurFrame;
	nextFrame[1] = g_TweenFrames.next.iNextFrame;
	ratio[1] = g_TweenFrames.next.fRatio;

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

        matrix result = lerp(curr, next, ratio[0]);
		
		/* Tweening */
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
            result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
        }

		transform += mul(result, weights[i]);
	}

	return transform;
}

matrix GetAnimationMatrixEdited(VS_IN input)
{
	float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
	float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2];

	/* cur */
	animIndex[0] = g_TweenFrames.cur.iAnimIndex;
	currFrame[0] = g_TweenFrames.cur.iCurFrame;
	nextFrame[0] = g_TweenFrames.cur.iNextFrame;
	ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
	animIndex[1] = g_TweenFrames.next.iAnimIndex;
	currFrame[1] = g_TweenFrames.next.iCurFrame;
	nextFrame[1] = g_TweenFrames.next.iNextFrame;
	ratio[1] = g_TweenFrames.next.fRatio;

	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;

	for (int i = 0; i < 4; i++)
	{
		float3 fSourceScale, fDestScale, fLerpScale;
		float4 fSourceRot, fDestRot, fLerpRot;
		float3 fSourcePos, fDestPos, fLerpPos;

		/* cur */
		{
			c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0)); /* Scale */
			c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0)); /* Quat */
			c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0)); /* Pos */
			c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0)); /* None */

			fSourceScale.x = c0.x;
			fSourceScale.y = c0.y;
			fSourceScale.z = c0.z;

			fSourceRot.x = c1.x;
			fSourceRot.y = c1.y;
			fSourceRot.z = c1.z;
			fSourceRot.w = c1.w;

			fSourcePos.x = c2.x;
			fSourcePos.y = c2.y;
			fSourcePos.z = c2.z;
		}

		/* next */
		{
			n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
			n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
			n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
			n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
			
			fDestScale.x = n0.x;
			fDestScale.y = n0.y;
			fDestScale.z = n0.z;

			fDestRot.x = n1.x;
			fDestRot.y = n1.y;
			fDestRot.z = n1.z;
			fDestRot.w = n1.w;

			fDestPos.x = n2.x;
			fDestPos.y = n2.y;
			fDestPos.z = n2.z;
		}

		/* Lerp */
		{
			fLerpScale = lerp(fSourceScale, fDestScale, ratio[0]);
            fLerpRot = q_slerp(fSourceRot, fDestRot, ratio[0]);
			fLerpPos = lerp(fSourcePos, fDestPos, ratio[0]);
		}

		/* AffineTransformation */
		{
			matrix scaleMatrix = 0;
			{
				scaleMatrix[0][0] = fLerpScale.x;
				scaleMatrix[1][1] = fLerpScale.y;
				scaleMatrix[2][2] = fLerpScale.z;
				scaleMatrix[3][3] = 1.f;
			}
			matrix translationToOrigin = 0;
			{			
				translationToOrigin[0][0] = 1.f;
				translationToOrigin[1][1] = 1.f;
				translationToOrigin[2][2] = 1.f;
				translationToOrigin[3][3] = 1.f;
			}
			matrix translationBack = translationToOrigin;
			{
				//translationBack[3][0] = 1.f;
				//translationBack[3][1] = 1.f;
				//translationBack[3][2] = 1.f;
				//translationBack[3][3] = 1.f;
			}
			matrix rotationMatrix = 0;
			{
				float x = fLerpRot.x;
				float y = fLerpRot.y;
				float z = fLerpRot.z;
				float w = fLerpRot.w;

				float xx = x * x;
				float yy = y * y;
				float zz = z * z;
				float xy = x * y;
				float xz = x * z;
				float yz = y * z;
				float wx = w * x;
				float wy = w * y;
				float wz = w * z;

				rotationMatrix[0][0] = 1.0f - 2.0f * (yy + zz);
				rotationMatrix[1][0] = 2.0f * (xy + wz);
				rotationMatrix[2][0] = 2.0f * (xz - wy);
				rotationMatrix[3][0] = 0.f;

				rotationMatrix[0][1] = 2.0f * (xy - wz);
				rotationMatrix[1][1] = 1.0f - 2.0f * (xx + zz);
				rotationMatrix[2][1] = 2.0f * (yz + wx);
				rotationMatrix[3][1] = 0.f;

				rotationMatrix[0][2] = 2.0f * (xz + wy);
				rotationMatrix[1][2] = 2.0f * (yz - wx);
				rotationMatrix[2][2] = 1.0f - 2.0f * (xx + yy);
				rotationMatrix[3][2] = 0.f;

				rotationMatrix[0][3] = 0.f;
				rotationMatrix[1][3] = 0.f;
				rotationMatrix[2][3] = 0.f;
				rotationMatrix[3][3] = 1.f;
			}
			matrix translationMatrix = 0;
			{
				translationMatrix[0][0] = 1.f;
				translationMatrix[1][1] = 1.f;
				translationMatrix[2][2] = 1.f;
				
				translationMatrix[0][3] = fLerpPos.x;
				translationMatrix[1][3] = fLerpPos.y;
				translationMatrix[2][3] = fLerpPos.z;
				translationMatrix[3][3] = 1.f;
			}

			curr = scaleMatrix * translationBack * rotationMatrix * translationToOrigin * translationMatrix;
		}



		matrix result = curr;
		//matrix result = lerp(curr, next, ratio[0]);

		/* if next */
		/*if (animIndex[1] >= 0)
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
			result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
		}*/

		transform += mul(result, weights[i]);
	}

	return transform;
}

matrix GetAnimationMatrix_2(VS_IN input)
{
    float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
    float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];

	/* cur */
    animIndex[0] = g_TweenFrames.cur.iAnimIndex;
    currFrame[0] = g_TweenFrames.cur.iCurFrame;
    nextFrame[0] = g_TweenFrames.cur.iNextFrame;
    ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
    animIndex[1] = g_TweenFrames.next.iAnimIndex;
    currFrame[1] = g_TweenFrames.next.iCurFrame;
    nextFrame[1] = g_TweenFrames.next.iNextFrame;
    ratio[1] = g_TweenFrames.next.fRatio;

    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix transform = 0;

    for (int i = 0; i < 4; i++)
    {
        float3 fCurAnimSourceScale, fCurAnimDestScale, fCurAnimLerpScale;
        float4 fCurAnimSourceRot, fCurAnimDestRot, fCurAnimLerpRot;
        float3 fCurAnimSourcePos, fCurAnimDestPos, fCurAnimLerpPos;
		
        float4x4 fCurAnimCurFrame = 0;
        float4x4 fCurAnimNextFrame = 0;
		
        matrix result;
		
		/* curr Frame */
		{
			c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
			c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
			c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
			c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		
			fCurAnimCurFrame = float4x4(c0, c1, c2, c3);
			decompose(fCurAnimCurFrame, fCurAnimSourcePos, fCurAnimSourceRot, fCurAnimSourceScale);
        }

		/* next Frame */ 
		{
			n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
			n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
			n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
			n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		
			fCurAnimNextFrame = float4x4(n0, n1, n2, n3);
			decompose(fCurAnimNextFrame, fCurAnimDestPos, fCurAnimDestRot, fCurAnimDestScale);
        }
		
		/* Frame Lerp */
		{
			fCurAnimLerpScale	= lerp(fCurAnimSourceScale, fCurAnimDestScale, ratio[0]);
			fCurAnimLerpRot		= q_slerp(fCurAnimSourceRot, fCurAnimDestRot, ratio[0]);
			fCurAnimLerpPos		= lerp(fCurAnimSourcePos, fCurAnimDestPos, ratio[0]);
        }
        
		/* if next */
        if (animIndex[1] >= 0)
        {
            float3 fNextAnimSourceScale, fNextAnimDestScale, fNextAnimLerpScale;
            float4 fNextAnimSourceRot, fNextAnimDestRot, fNextAnimLerpRot;
            float3 fNextAnimSourcePos, fNextAnimDestPos, fNextAnimLerpPos;
			
            float4x4 fNextAnimCurFrame = 0;
            float4x4 fNextAnimNextFrame = 0;
			
			/* curr Frame */
			{
				c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
				c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
				c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
				c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			
				fNextAnimCurFrame = float4x4(c0, c1, c2, c3);
				decompose(fNextAnimCurFrame, fNextAnimSourcePos, fNextAnimSourceRot, fNextAnimSourceScale);
            }
			
			/* next Frame */ 
			{
                n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
				n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
				n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
				n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));

				fNextAnimNextFrame = float4x4(n0, n1, n2, n3);
				decompose(fNextAnimNextFrame, fNextAnimDestPos, fNextAnimDestRot, fNextAnimDestScale);
            }
			
			/* Frame Lerp */			
			{
				fNextAnimLerpScale	= lerp(fNextAnimSourceScale, fNextAnimDestScale, ratio[1]);
				fNextAnimLerpRot	= q_slerp(fNextAnimSourceRot, fNextAnimDestRot, ratio[1]);
				fNextAnimLerpPos	= lerp(fNextAnimSourcePos, fNextAnimDestPos, ratio[1]);
            }
			
			/* Tween Lerp */			
			{
				float3 fTweenScale	= lerp(fCurAnimLerpScale, fNextAnimLerpScale, g_TweenFrames.fTweenRatio);
				float4 fTweenRot	= q_slerp(fCurAnimLerpRot, fNextAnimLerpRot, g_TweenFrames.fTweenRatio);
				float3 fTweenPos	= lerp(fCurAnimLerpPos, fNextAnimLerpPos, g_TweenFrames.fTweenRatio);
            
				result = compose(fTweenPos, fTweenRot, fTweenScale);
            }
        }
        else
        {
            result = compose(fCurAnimLerpPos, fCurAnimLerpRot, fCurAnimLerpScale);
        }

        transform += mul(result, weights[i]);
    }

    return transform;
}

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	//float4x4	BoneMatrix = GetAnimationMatrix_2(In);
	float4x4	BoneMatrix = GetAnimationMatrix(In);

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector		vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
	Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal.xyz, Out.vTangent));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

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
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);


	float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 2.f);

	vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;

	if (0.f == Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_NORMAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	
	Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);


	vector		vTextureNormal = g_NormalTexture.Sample(ModelSampler, In.vTexUV);

	float3		vNormal = vTextureNormal.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	
	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);

	

	float fRimPower = 1.f - saturate(dot(vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 2.f);

	vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;

	if (0.f == Out.vDiffuse.a)
		discard;

	return Out;
}



PS_OUT PS_DISSOLVE_DEAD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vDissolve = g_DissolveTexture.Sample(ModelSampler, In.vTexUV);

	if (vDissolve.r <= g_fDissolveWeight)
		discard;

	if ((vDissolve.r - g_fDissolveWeight) < 0.1f)
		Out.vDiffuse = g_vDissolveColor;
	else if((vDissolve.r - g_fDissolveWeight) < 0.115f)
		Out.vDiffuse = g_vDissolveColor - 0.1f;
	else if ((vDissolve.r - g_fDissolveWeight) < 0.125f)
		Out.vDiffuse = g_vDissolveColor - 0.1f;
	else
		Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);

	

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);

	if (0 == Out.vDiffuse.a)
		discard;

	return Out;
}


// 그림자 픽셀 쉐이더.
struct PS_OUT_SHADOW_DEPTH
{
	float4		vDepth : SV_TARGET0;
};


PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
	PS_OUT_SHADOW_DEPTH Out = (PS_OUT_SHADOW_DEPTH)0;

	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if(vColor.a <= 0.01f)
		discard;


	Out.vDepth = vector(In.vProjPos.w / 1000.0f, 0.f, 0.f, 1.f);

	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		// 0
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalPass
	{
		// 1
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass DissolveDead
	{
		// 2
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE_DEAD();
	}

	pass Temp0
	{
		// 3
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp1
	{
		// 4
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp2
	{
		// 5
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp3
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp4
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp5
	{
		// 7
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp6
	{
		// 8
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Shadow_Depth
	{
		// 10
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Shadow, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_DEPTH();
	}
};