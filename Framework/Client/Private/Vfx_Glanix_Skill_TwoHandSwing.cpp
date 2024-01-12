#include "stdafx.h"
#include "Vfx_Glanix_Skill_TwoHandSwing.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Glanix_Skill_TwoHandSwing::CVfx_Glanix_Skill_TwoHandSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_TwoHandSwing::CVfx_Glanix_Skill_TwoHandSwing(const CVfx_Glanix_Skill_TwoHandSwing& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_START]    = 0;
	m_pPositionOffset[TYPE_START] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_START]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_START] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_TwoHandSwing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_START && m_iOwnerFrame >= m_pFrameTriger[TYPE_START])
		{
			//GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT(""),
			//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_START], m_pScaleOffset[TYPE_START], m_pRotationOffset[TYPE_START]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_TwoHandSwing::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_TwoHandSwing* CVfx_Glanix_Skill_TwoHandSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_TwoHandSwing* pInstance = new CVfx_Glanix_Skill_TwoHandSwing(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_TwoHandSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_TwoHandSwing::Clone(void* pArg)
{
	CVfx_Glanix_Skill_TwoHandSwing* pInstance = new CVfx_Glanix_Skill_TwoHandSwing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_TwoHandSwing");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_TwoHandSwing::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}