#include "stdafx.h"
#include "Vfx_SwordMan_Skill_SwordTempest.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Character_Projectile.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Character_Manager.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_SwordMan_Skill_SwordTempest::CVfx_SwordMan_Skill_SwordTempest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_SwordTempest::CVfx_SwordMan_Skill_SwordTempest(const CVfx_SwordMan_Skill_SwordTempest& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_D_SQUARE]    = 0;
	m_pPositionOffset[TYPE_D_SQUARE] = _float3(0.f, 0.f, 0.5f);
	m_pScaleOffset[TYPE_D_SQUARE]    = _float3(4.f, 5.f, 8.f);
	m_pRotationOffset[TYPE_D_SQUARE] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_ATTACK_01]    = 9;
	m_pPositionOffset[TYPE_E_ATTACK_01] = _float3(0.f, 0.1f, -0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_01]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[TYPE_E_ATTACK_01] = _float3(0.f, -15.f, -20.f);

	// 2
	m_pFrameTriger[TYPE_P_SPARKLE_01]    = 12;
	m_pPositionOffset[TYPE_P_SPARKLE_01] = _float3(0.f, 0.f, 0.0f);
	m_pScaleOffset[TYPE_P_SPARKLE_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SPARKLE_01] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_E_ATTACK_02]    = 16;
	m_pPositionOffset[TYPE_E_ATTACK_02] = _float3(0.f, 0.1f, -0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_02]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[TYPE_E_ATTACK_02] = _float3(0.f, -15.f, 0.f);

	// 4
	m_pFrameTriger[TYPE_P_XSPARKLE_01]    = 20;
	m_pPositionOffset[TYPE_P_XSPARKLE_01] = _float3(0.f, 0.8f, 0.f);
	m_pScaleOffset[TYPE_P_XSPARKLE_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_XSPARKLE_01] = _float3(0.f, 0.f, 0.f);

	// 5
	m_pFrameTriger[TYPE_E_ATTACK_03]    = 25;
	m_pPositionOffset[TYPE_E_ATTACK_03] = _float3(0.f, 0.1f, -0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_03]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[TYPE_E_ATTACK_03] = _float3(0.f, 0.f, -35.f);

	// 6
	m_pFrameTriger[TYPE_P_SPARKLE_02]    = 29;
	m_pPositionOffset[TYPE_P_SPARKLE_02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SPARKLE_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SPARKLE_02] = _float3(0.f, 0.f, 0.f);

	// 7
	m_pFrameTriger[TYPE_E_ATTACK_04]    = 33;
	m_pPositionOffset[TYPE_E_ATTACK_04] = _float3(0.f, 0.1f, -0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_04]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[TYPE_E_ATTACK_04] = _float3(0.f, -20.f, 35.f);

	// 8
	m_pFrameTriger[TYPE_P_XSPARKLE_02]    = 37;
	m_pPositionOffset[TYPE_P_XSPARKLE_02] = _float3(0.f, 0.8f, 0.f);
	m_pScaleOffset[TYPE_P_XSPARKLE_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_XSPARKLE_02] = _float3(0.f, 0.f, 0.f);

	// 9
	m_pFrameTriger[TYPE_E_ATTACK_05]    = 45;
	m_pPositionOffset[TYPE_E_ATTACK_05] = _float3(0.f, 0.1f, -0.5f);
	m_pScaleOffset[TYPE_E_ATTACK_05]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[TYPE_E_ATTACK_05] = _float3(0.f, 0.f, 30.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_SwordTempest::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (-1 == m_iType)
		{
			CCharacter* pPlayer = static_cast<CCharacter*>(m_pOwnerObject);
			if (nullptr == pPlayer)
				MSG_BOX("Casting_Failde");
			else
				m_iType = pPlayer->Get_ElementalType();

			switch (m_iType)
			{
			case ELEMENTAL_TYPE::FIRE:
				m_fMainColor = _float3(1.f, 0.45f, 0.2f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.28f, 0.720f, 1.f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.140f, 1.f, 0.559f);
				break;
			}
		}

		if (m_iCount == TYPE_D_SQUARE && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_SQUARE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_SQUARE], m_pScaleOffset[TYPE_D_SQUARE], m_pRotationOffset[TYPE_D_SQUARE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_ATTACK_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_01])
		{
			CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
			ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);

			CGameObject* pAuraBlade = GI->Clone_GameObject(TEXT("Prototype_GameObject_SwordMan_AuraBlade"), LAYER_CHARACTER, &ProjectileDesc);
			pAuraBlade->Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);

			CTransform* pAuraBladeTransform = pAuraBlade->Get_Component<CTransform>(L"Com_Transform");

			_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(WorldMatrix.r[CTransform::STATE_POSITION]), XMVectorGetY(WorldMatrix.r[CTransform::STATE_POSITION]) + 0.8f, XMVectorGetZ(WorldMatrix.r[CTransform::STATE_POSITION]), 1.f);

			pAuraBladeTransform->Set_WorldMatrix(WorldMatrix);
			GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_CHARACTER, pAuraBlade);
			//
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SwordTempest_Trail_L"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_01], m_pScaleOffset[TYPE_E_ATTACK_01], m_pRotationOffset[TYPE_E_ATTACK_01], pAuraBlade, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			//
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_03"),
				XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pAuraBlade, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}

		// 
		else if (m_iCount == TYPE_P_SPARKLE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_01])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_01"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_01], m_pScaleOffset[TYPE_P_SPARKLE_01], m_pRotationOffset[TYPE_P_SPARKLE_01], nullptr, &pParticle);
			if(nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}
		//

		else if (m_iCount == TYPE_E_ATTACK_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_02])
		{
			CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
			ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);
			CGameObject* pAuraBlade = GI->Clone_GameObject(TEXT("Prototype_GameObject_SwordMan_AuraBlade"), LAYER_CHARACTER, &ProjectileDesc);
			pAuraBlade->Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
			CTransform* pAuraBladeTransform = pAuraBlade->Get_Component<CTransform>(L"Com_Transform");

			_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(WorldMatrix.r[CTransform::STATE_POSITION]), XMVectorGetY(WorldMatrix.r[CTransform::STATE_POSITION]) + 0.8f, XMVectorGetZ(WorldMatrix.r[CTransform::STATE_POSITION]), 1.f);

			pAuraBladeTransform->Set_WorldMatrix(WorldMatrix);
			GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_CHARACTER, pAuraBlade);
			//
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SwordTempest_Trail_R"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_02], m_pScaleOffset[TYPE_E_ATTACK_02], m_pRotationOffset[TYPE_E_ATTACK_02], pAuraBlade, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			//
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_03"),
				XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pAuraBlade, &pParticle);
			if (nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}

		//
		else if (m_iCount == TYPE_P_XSPARKLE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_XSPARKLE_01])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_XSPARKLE_01], m_pScaleOffset[TYPE_P_XSPARKLE_01], m_pRotationOffset[TYPE_P_XSPARKLE_01], nullptr, &pParticle);
			if(nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}
		//

		else if (m_iCount == TYPE_E_ATTACK_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_03])
		{
			CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
			ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);
			CGameObject* pAuraBlade = GI->Clone_GameObject(TEXT("Prototype_GameObject_SwordMan_AuraBlade"), LAYER_CHARACTER, &ProjectileDesc);
			pAuraBlade->Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
			CTransform* pAuraBladeTransform = pAuraBlade->Get_Component<CTransform>(L"Com_Transform");

			_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(WorldMatrix.r[CTransform::STATE_POSITION]), XMVectorGetY(WorldMatrix.r[CTransform::STATE_POSITION]) + 0.8f, XMVectorGetZ(WorldMatrix.r[CTransform::STATE_POSITION]), 1.f);

			pAuraBladeTransform->Set_WorldMatrix(WorldMatrix);
			GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_CHARACTER, pAuraBlade);
			//
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SwordTempest_Trail_L"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_03], m_pScaleOffset[TYPE_E_ATTACK_03], m_pRotationOffset[TYPE_E_ATTACK_03], pAuraBlade, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			//
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_03"),
				XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pAuraBlade, &pParticle);
			if (nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}

		//
		else if (m_iCount == TYPE_P_SPARKLE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_02])
		{
		    CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_02], m_pScaleOffset[TYPE_P_SPARKLE_02], m_pRotationOffset[TYPE_P_SPARKLE_02], nullptr, &pParticle);
			if (nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}
		//

		else if (m_iCount == TYPE_E_ATTACK_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_04])
		{
			CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
			ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);
			CGameObject* pAuraBlade = GI->Clone_GameObject(TEXT("Prototype_GameObject_SwordMan_AuraBlade"), LAYER_CHARACTER, &ProjectileDesc);
			pAuraBlade->Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
			CTransform* pAuraBladeTransform = pAuraBlade->Get_Component<CTransform>(L"Com_Transform");

			_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(WorldMatrix.r[CTransform::STATE_POSITION]), XMVectorGetY(WorldMatrix.r[CTransform::STATE_POSITION]) + 0.8f, XMVectorGetZ(WorldMatrix.r[CTransform::STATE_POSITION]), 1.f);

			pAuraBladeTransform->Set_WorldMatrix(WorldMatrix);
			GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_CHARACTER, pAuraBlade);
			//
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SwordTempest_Trail_R"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_04], m_pScaleOffset[TYPE_E_ATTACK_04], m_pRotationOffset[TYPE_E_ATTACK_04], pAuraBlade, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			//
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_03"),
				XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pAuraBlade, &pParticle);
			if (nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}

		//
		else if (m_iCount == TYPE_P_XSPARKLE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_XSPARKLE_02])
		{
		    CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_XSPARKLE_02], m_pScaleOffset[TYPE_P_XSPARKLE_02], m_pRotationOffset[TYPE_P_XSPARKLE_02], nullptr, &pParticle);
			if (nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}
        //

		else if (m_iCount == TYPE_E_ATTACK_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_05])
		{
			CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
			ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN);
			CGameObject* pAuraBlade = GI->Clone_GameObject(TEXT("Prototype_GameObject_SwordMan_AuraBlade"), LAYER_CHARACTER, &ProjectileDesc);
			pAuraBlade->Set_Collider_AttackMode(CCollider::ATTACK_TYPE::BLOW, 0.f, 0.f, 0.f, false);
			CTransform* pAuraBladeTransform = pAuraBlade->Get_Component<CTransform>(L"Com_Transform");

			_matrix	WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(WorldMatrix.r[CTransform::STATE_POSITION]), XMVectorGetY(WorldMatrix.r[CTransform::STATE_POSITION]) + 0.8f, XMVectorGetZ(WorldMatrix.r[CTransform::STATE_POSITION]), 1.f);

			pAuraBladeTransform->Set_WorldMatrix(WorldMatrix);
			GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_CHARACTER, pAuraBlade);
			//
			CEffect* pEffectL = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SwordTempest_Trail_L"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_05], m_pScaleOffset[TYPE_E_ATTACK_05], m_pRotationOffset[TYPE_E_ATTACK_05], pAuraBlade, &pEffectL);
			if (nullptr != pEffectL)
			{
				pEffectL->Set_Color(m_fMainColor);
				pEffectL->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			//
			_float3 fRotationOffset = m_pRotationOffset[TYPE_E_ATTACK_05];
			fRotationOffset.x *= -1.f;
			fRotationOffset.y *= -1.f;
			fRotationOffset.z *= -1.f;
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SwordTempest_Trail_R"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_05], m_pScaleOffset[TYPE_E_ATTACK_05], fRotationOffset, pAuraBlade, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			//
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SwordTempest_Sparkle_Circle_03"),
				XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pAuraBlade, &pParticle);
			if (nullptr != pParticle)
				pParticle->Set_Color(m_fMainColor);
			m_iCount++;
		}


		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_SwordTempest::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_SwordTempest* CVfx_SwordMan_Skill_SwordTempest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_SwordTempest* pInstance = new CVfx_SwordMan_Skill_SwordTempest(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_SwordTempest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_SwordTempest::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_SwordTempest* pInstance = new CVfx_SwordMan_Skill_SwordTempest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SwordTempest");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_SwordTempest::Free()
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