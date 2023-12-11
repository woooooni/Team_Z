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
	/* Ư�� �ִϸ��̼ǿ��� ���Ǵ� ���� �����̴�. */
	/* �� �̸��� ���� ������ �ִ� HierarchyNodes�� ���� �� �ѳ�� �̸��� ���� ���̴�. */
	/* �� �̸����� ���� �̸��� ���� HierarchyNodes�� ä�ο� �����صд�. */
	/* �� �����ϴ�? : ä���� ���� Ű�������� �ð��뿡 �´� Ű�ÿ��Ӥ� ���¸� �����. �̰ɷ� ��� �����.
	�̷��� ���� ����� erarchyNodes�� �����س�����. */
	m_strName = CUtils::ToWString(pAIChannel->mNodeName.C_Str());

	//m_pHierarchyNode = pModel->Get_HierarchyNode(m_szName);
	//if (nullptr == m_pHierarchyNode)
	//	return E_FAIL;

	// Safe_AddRef(m_pHierarchyNode);


	/* Ű������ �����鸦 �ε��Ѵ�. */
	/* Ű������ : ��ü�ִϸ��̼� ���� ��, Ư�� �ð��뿡 �� �̻��� ǥ���ؾ��� ������ ���� ��������̴�. */

	/* �� Ű������ ������ ũ��, ȸ��, �̵����� ������ �ٸ� �� �ִ�. */
	/* ������ �ٸ���? : �����ѳ��� ���� �ð��뿡 Ű�����Ӥ� ���¸� ������ �����̴�. */
	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	/* �� ������ ���� �ۿ� ����������? ���� ������ ��� �����س���
	���� ������ ���¸� �̿��ؼ� Ű������ ���¸� ����� ����. */
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


_uint CChannel::Update_Transformation(_float fPlayTime, _float fTimeDelta, _uint iCurrentKeyFrame, CTransform* pTransform, CHierarchyNode* pNode, __out _float* pRatio)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	/* ������ Ű�������̻����� �Ѿ���� : ������ Ű������ �ڼ��� ������ �� �ֵ��� �Ѵ�. */
	if (fPlayTime >= m_KeyFrames.back().fTime)
	{
		vScale = m_KeyFrames.back().vScale;
		vRotation = m_KeyFrames.back().vRotation;
		vPosition = m_KeyFrames.back().vPosition;
	}

	/* Ư�� Ű�����Ӱ� Ű������ ���̿� �����Ѵ�. */
	/* 1�� �������� �󸶳� ����Ǿ�����(Ű�����Ӱ� Ű������ ���̸�)�� Ȯ���Ѵ�.( Ratio) */
	/* ������Ratio���� ���� �� Ű�����ӻ����� ��������� ����� ����. */
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

_uint CChannel::Interpolation(_float fPlayTime, _float fTimeDelta, CAnimation* pCurrAnimation, CAnimation* pNextAnimation, CTransform* pTransform, _uint iCurrentKeyFrame, CHierarchyNode* pNode, CModel* pModel, __out _float* pRatio)
{
	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	m_fInterpolationTime += fTimeDelta;

	if (m_fInterpolationTime >= 0.2f)
	{
		// TODO : No Interpolation & Set Next Animation
		if(pModel->Is_InterpolatingAnimation())
			pModel->Complete_Interpolation();

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
