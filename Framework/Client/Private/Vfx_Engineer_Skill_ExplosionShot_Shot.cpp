#include "stdafx.h"
#include "Vfx_Engineer_Skill_ExplosionShot_Shot.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Character_Projectile.h"

CVfx_Engineer_Skill_ExplosionShot_Shot::CVfx_Engineer_Skill_ExplosionShot_Shot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_ExplosionShot_Shot::CVfx_Engineer_Skill_ExplosionShot_Shot(const CVfx_Engineer_Skill_ExplosionShot_Shot& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Shot::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 총알 발사
	{
		// 총알 오브젝트
		m_pFrameTriger[TYPE_ET1_O_BULLET] = 0;
		m_pPositionOffset[TYPE_ET1_O_BULLET] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_O_BULLET]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_O_BULLET] = _float3(0.f, 0.f, 0.f);

		// 점점 커지는 원라인 이펙트 -> 색 회색으로 변함
		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 16;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		// 작은 원 파티클들 몇개
		m_pFrameTriger[TYPE_ET1_E_CIRCLES] = 16;
		m_pPositionOffset[TYPE_ET1_E_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLES] = _float3(0.f, 0.f, 0.f);

		// 불꽃 파티클
		m_pFrameTriger[TYPE_ET1_P_FIRE] = 16;
		m_pPositionOffset[TYPE_ET1_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_FIRE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_FIRE] = _float3(0.f, 0.f, 0.f);

		// 연기 파티클
		m_pFrameTriger[TYPE_ET1_P_SMOKE] = 16;
		m_pPositionOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SMOKE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Shot::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_ExplosionShot_Shot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_O_BULLET && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_O_BULLET])
		{
			Create_Bullet();
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{

			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLES])
		{

			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_FIRE])
		{

			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SMOKE])
		{

			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_ExplosionShot_Shot::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Shot::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Shot::Ready_Components()
{
	return S_OK;
}

void CVfx_Engineer_Skill_ExplosionShot_Shot::Create_Bullet()
{
	CCharacter* pPlayer = static_cast<CCharacter*>(m_pOwnerObject);
	if (nullptr == pPlayer)
		return;

	CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc;
	ProjectileDesc.pOwner = pPlayer;

	CGameObject* pBullet = GI->Clone_GameObject(L"Prototype_GameObject_Engineer_Bullet_Bomb", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
	if (nullptr == pBullet)
		return;

	CTransform* pBulletTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pBulletTransform)
		return;

	pBulletTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_WorldMatrix));

	Vec3 vScale = pBulletTransform->Get_Scale();
	pBulletTransform->Set_Scale(vScale);

	Vec4 vStartPosition = pBulletTransform->Get_Position();
	vStartPosition += XMVector3Normalize(pBulletTransform->Get_Look()) * 0.5f;
	vStartPosition.y += 0.7f;
	pBulletTransform->Set_State(CTransform::STATE_POSITION, vStartPosition);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pBullet)))
		MSG_BOX("Generate Bullet Failed.");
}

CVfx_Engineer_Skill_ExplosionShot_Shot* CVfx_Engineer_Skill_ExplosionShot_Shot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_ExplosionShot_Shot* pInstance = new CVfx_Engineer_Skill_ExplosionShot_Shot(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_ExplosionShot_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_ExplosionShot_Shot::Clone(void* pArg)
{
	CVfx_Engineer_Skill_ExplosionShot_Shot* pInstance = new CVfx_Engineer_Skill_ExplosionShot_Shot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_ExplosionShot_Shot");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_ExplosionShot_Shot::Free()
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