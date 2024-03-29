#include "stdafx.h"
#include "Vfx_Witch_Skill_Rage02Sphere_Ready.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Particle.h"
#include "Decal.h"

CVfx_Witch_Skill_Rage02Sphere_Ready::CVfx_Witch_Skill_Rage02Sphere_Ready(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_Rage02Sphere_Ready::CVfx_Witch_Skill_Rage02Sphere_Ready(const CVfx_Witch_Skill_Rage02Sphere_Ready& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_Rage02Sphere_Ready::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_SPHERE] = 0;
	m_pPositionOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE] = _float3(0.7f, 0.7f, 0.7f);
	m_pRotationOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMALL] = 0;
	m_pPositionOffset[TYPE_P_SMALL] = _float3(0.f, 1.7f, 0.f);
	m_pScaleOffset[TYPE_P_SMALL] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMALL] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_D_WARNING] = 0;
	m_pPositionOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_WARNING] = _float3(2.5f, 2.f, 2.5f);
	m_pRotationOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Skill_Rage02Sphere_Ready::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_Rage02Sphere_Ready::Tick(_float fTimeDelta)
{
	// 구 최초 생성
	if (!m_bIsCreate && m_pSphere == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Rage02_Sphere"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE], m_pScaleOffset[TYPE_E_SPHERE], m_pRotationOffset[TYPE_E_SPHERE], nullptr, &m_pSphere);

		m_vPos = m_pSphere->Get_TransformCom()->Get_Position();
		m_tLerpDesc.Start(m_vPos.y, m_fMaxY, m_fDuration, LERP_MODE::SMOOTHER_STEP);

		m_bIsCreate = true;
	}

	if (!m_bIsCreateParticle)
	{
		m_fAccTime += fTimeDelta;
		if (m_fAccTime > m_fCreateParticleTime)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Witch_Rage02Sphere"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_WARNING], m_pScaleOffset[TYPE_D_WARNING], m_pRotationOffset[TYPE_D_WARNING], nullptr, &m_pWarningDecal);

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_Rage02Sphere_Small"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMALL], m_pScaleOffset[TYPE_P_SMALL], m_pRotationOffset[TYPE_P_SMALL], nullptr, &m_pParticle);
			m_bIsCreateParticle = true;
		}
	}

	// 구 이펙트가 끝났다면
	if (m_pSphere != nullptr)
	{
		if (m_tLerpDesc.bActive)
		{
			m_vPos.y = m_tLerpDesc.Update(fTimeDelta);
			m_pSphere->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vPos);
		}
		//else
		//{
		//	m_vPos = m_pSphere->Get_TransformCom()->Get_Position();
		//	if (m_vPos.y > m_fDestY)
		//	{
		//		m_vPos.y -= fTimeDelta;
		//		m_pSphere->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vPos);
		//	}
		//}

		if (m_pSphere->Is_Dead() || m_pSphere->Get_DieEffect())
		{
			Set_Dead(true);
			m_pParticle->Set_Dead(true);
			m_pWarningDecal->Set_Dead(true);
		}
	}
}

void CVfx_Witch_Skill_Rage02Sphere_Ready::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_Rage02Sphere_Ready::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_Rage02Sphere_Ready::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_Rage02Sphere_Ready* CVfx_Witch_Skill_Rage02Sphere_Ready::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_Rage02Sphere_Ready* pInstance = new CVfx_Witch_Skill_Rage02Sphere_Ready(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_Rage02Sphere_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_Rage02Sphere_Ready::Clone(void* pArg)
{
	CVfx_Witch_Skill_Rage02Sphere_Ready* pInstance = new CVfx_Witch_Skill_Rage02Sphere_Ready(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_Rage02Sphere_Ready");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_Rage02Sphere_Ready::Free()
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