#include "..\Public\Channel.h"
#include "Model.h"
#include "HierarchyNode.h"
#include "Utils.h"
#include "Animation.h"
#include "Transform.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim* pAIChannel)
{
	/* 특정 애니메이션에서 사용되는 뼈의 정보이다. */
	/* 이 이름은 모델이 가지고 있는 HierarchyNodes의 뼈대 중 한놈과 이름이 같을 것이다. */
	/* 이 이름으로 같은 이름을 가진 HierarchyNodes를 채널에 보관해둔다. */
	/* 왜 보관하니? : 채널이 가진 키프레임중 시간대에 맞는 키플에ㅣㅁ 상태를 만들고. 이걸로 행렬 만들고.
	이렇게 만든 행렬을 erarchyNodes에 저장해놔야해. */
	m_strName = CUtils::ToWString(pAIChannel->mNodeName.C_Str());

	//m_pHierarchyNode = pModel->Get_HierarchyNode(m_szName);
	//if (nullptr == m_pHierarchyNode)
	//	return E_FAIL;

	// Safe_AddRef(m_pHierarchyNode);


	/* 키프레임 정보들를 로드한다. */
	/* 키프레임 : 전체애니메이션 동작 중, 특정 시간대에 이 ㅜ뼈가 표현해야할 동작의 상태 행렬정보이다. */

	/* 이 키프레임 갯수는 크기, 회전, 이동마다 갯수가 다를 수 있다. */
	/* 개숫가 다르다? : 부족한놈은 이전 시간대에 키프레임ㅇ 상태를 가지기 때문이다. */
	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	/* 이 변수를 루프 밖에 선언한이유? 없는 상태인 경우 저장해놨던
	이전 루프의 상태를 이용해서 키프레임 상태를 만들기 위해. */
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	for (_uint i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME			KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mScalingKeys[i].mTime;
		}
		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = pAIChannel->mRotationKeys[i].mTime;
		}
		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}


_uint CChannel::Update_Transformation(_float fPlayTime, _uint iCurrentKeyFrame, CHierarchyNode* pNode, __out _float* pRatio)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	/* 마지막 키프레임이상으로 넘어갔을때 : 마지막 키프레임 자세로 고정할 수 있도록 한다. */
	if (fPlayTime >= m_KeyFrames.back().fTime)
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;
	}

	/* 특정 키프레임과 키프레임 사이에 존재한다. */
	/* 1을 기준으로 얼마나 재생되었는지(키프레임과 키프레임 사이를)를 확인한다.( Ratio) */
	/* 결정된Ratio값에 따라 두 키프레임사이의 상태행려를 만들어 낸다. */
	else
	{
		while (fPlayTime >= m_KeyFrames[iCurrentKeyFrame + 1].fTime)
			++iCurrentKeyFrame;

		_float		fRatio = (fPlayTime - m_KeyFrames[iCurrentKeyFrame].fTime) /
			(m_KeyFrames[iCurrentKeyFrame + 1].fTime - m_KeyFrames[iCurrentKeyFrame].fTime);

		if (nullptr != pRatio)
			*pRatio = fRatio;

		_float3		vSourScale, vDestScale;
		_float4		vSourRotation, vDestRotation;
		_float3		vSourPosition, vDestPosition;

		vSourScale = m_KeyFrames[iCurrentKeyFrame].vScale;
		vDestScale = m_KeyFrames[iCurrentKeyFrame + 1].vScale;

		vSourRotation = m_KeyFrames[iCurrentKeyFrame].vRotation;
		vDestRotation = m_KeyFrames[iCurrentKeyFrame + 1].vRotation;

		vSourPosition = m_KeyFrames[iCurrentKeyFrame].vPosition;
		vDestPosition = m_KeyFrames[iCurrentKeyFrame + 1].vPosition;

		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vSourPosition), XMLoadFloat3(&vDestPosition), fRatio));
	}


	_matrix	TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

	if (nullptr != pNode)
	{
		pNode->Set_Transformation(TransformationMatrix);
	}
		

	m_iCurrFrames = iCurrentKeyFrame;
	return iCurrentKeyFrame;
}

_uint CChannel::Update_Transformation_NoneLerp(_uint iCurrentKeyFrame, CHierarchyNode* pNode)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	/* 마지막 키프레임이상으로 넘어갔을때 : 마지막 키프레임 자세로 고정할 수 있도록 한다. */
	if (iCurrentKeyFrame < m_KeyFrames.size())
	{
		vScale = m_KeyFrames[iCurrentKeyFrame].vScale;
		vRotation = m_KeyFrames[iCurrentKeyFrame].vRotation;
		vPosition = m_KeyFrames[iCurrentKeyFrame].vPosition;
	}
	else
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;
	}

	Matrix TransformationMatrix;

	// << : Shader SRT Test 
	/*memcpy(&TransformationMatrix.m[0], &vScale, sizeof(Vec3));
	memcpy(&TransformationMatrix.m[1], &vRotation, sizeof(Vec4));
	memcpy(&TransformationMatrix.m[2], &vPosition, sizeof(Vec3));

	if (nullptr != pNode)
		pNode->Set_Transformation(TransformationMatrix);

	return iCurrentKeyFrame;*/
	// >> : 

	/* 기존 아핀 트랜스폼 생성 함수 */
	{
		TransformationMatrix = XMMatrixAffineTransformation(
			XMLoadFloat3(&vScale), 
			XMVectorSet(0.f, 0.f, 0.f, 1.f), 
			XMLoadFloat4(&vRotation), 
			XMVectorSetW(XMLoadFloat3(&vPosition), 1.f)
		);

		if (nullptr != pNode)
			pNode->Set_Transformation(TransformationMatrix);

		return iCurrentKeyFrame;
	}

	/* 커스텀 아핀 트랜스폼 생성 함수 */
	{
		// 크기 조절 행렬 생성
		XMMATRIX scaleMatrix = XMMatrixScalingFromVector(Vec3(vScale));

		// 회전 중심을 원점으로 이동
		XMMATRIX translationToOrigin = XMMatrixTranslationFromVector(XMVectorNegate(Vec4(0.f, 0.f, 0.f, 1.f)));
		XMMATRIX translationBack = XMMatrixTranslationFromVector(Vec4(0.f, 0.f, 0.f, 1.f));

		// 회전 행렬 생성
		XMMATRIX rotationMatrix;

		/* 기존 쿼터니언 생성 함수 */
		{
			//rotationMatrix = XMMatrixRotationQuaternion(Vec4(vRotation));
		}

		/* 커스텀 쿼터니언 생성 함수 */
		{
			// Extract quaternion components
			float x = XMVectorGetX(Vec4(vRotation));
			float y = XMVectorGetY(Vec4(vRotation));
			float z = XMVectorGetZ(Vec4(vRotation));
			float w = XMVectorGetW(Vec4(vRotation));

			// Calculate rotation matrix elements
			float xx = x * x;
			float yy = y * y;
			float zz = z * z;
			float xy = x * y;
			float xz = x * z;
			float yz = y * z;
			float wx = w * x;
			float wy = w * y;
			float wz = w * z;

			rotationMatrix.r[0] = XMVectorSet(
				1.0f - 2.0f * (yy + zz), 
				2.0f * (xy + wz), 
				2.0f * (xz - wy), 
				0.0f);

			rotationMatrix.r[1] = XMVectorSet(
				2.0f * (xy - wz), 
				1.0f - 2.0f * (xx + zz), 
				2.0f * (yz + wx), 
				0.0f);

			rotationMatrix.r[2] = XMVectorSet(
				2.0f * (xz + wy), 
				2.0f * (yz - wx), 
				1.0f - 2.0f * (xx + yy), 
				0.0f);

			rotationMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// 이동 행렬 생성
		XMMATRIX translationMatrix = XMMatrixTranslationFromVector(Vec3(vPosition));

		// 순서대로 행렬 곱셈을 수행하여 Affine 변환 행렬 생성
		TransformationMatrix = scaleMatrix * translationBack * rotationMatrix * translationToOrigin * translationMatrix;
	}

	if (nullptr != pNode)
		pNode->Set_Transformation(TransformationMatrix);

	return iCurrentKeyFrame;
}

_uint CChannel::Interpolation(_float fPlayTime, _float fTimeDelta, CAnimation* pCurrAnimation, CAnimation* pNextAnimation, CTransform* pTransform, _uint iCurrentKeyFrame, CHierarchyNode* pNode, CModel* pModel, __out _float* pRatio)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	m_fInterpolationTime += fTimeDelta;

	if (m_fInterpolationTime >= 0.2f)
	{
		// TODO : No Interpolation & Set Next Animation
		//if(pModel->Is_InterpolatingAnimation())
			//pModel->Complete_Interpolation();

		m_fInterpolationTime = 0.f;
		return iCurrentKeyFrame;
	}
	else
	{
		CChannel* pChannel = pNextAnimation->Get_Channel(m_strName);
		KEYFRAME NextKeyFrame = pChannel->Get_FirstFrame();

		
		_float		fRatio = m_fInterpolationTime / 0.2f;
		if (nullptr != pRatio)
			*pRatio = fRatio;

		_float3		vSourScale, vDestScale;
		_float4		vSourRotation, vDestRotation;
		_float3		vSourPosition, vDestPosition;

		vSourScale = m_KeyFrames[iCurrentKeyFrame].vScale;
		vDestScale = NextKeyFrame.vScale;

		vSourRotation = m_KeyFrames[iCurrentKeyFrame].vRotation;
		vDestRotation = NextKeyFrame.vRotation;

		vSourPosition = m_KeyFrames[iCurrentKeyFrame].vPosition;
		vDestPosition = NextKeyFrame.vPosition;

		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio));
		XMStoreFloat3(&vPosition, XMVectorLerp(XMLoadFloat3(&vSourPosition), XMLoadFloat3(&vDestPosition), fRatio));
	}

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMVectorSetW(XMLoadFloat3(&vPosition), 1.f));

	if (nullptr != pNode)
	{
		pNode->Set_Transformation(TransformationMatrix);
	}

	return iCurrentKeyFrame;
}




CChannel* CChannel::Create(aiNodeAnim* pAIChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel)))
	{
		MSG_BOX("Failed To Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Create_Bin()
{
	CChannel* pInstance = new CChannel;

	return pInstance;
}


void CChannel::Free()
{
	__super::Free();
}

