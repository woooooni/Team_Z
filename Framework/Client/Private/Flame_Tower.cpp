#include "stdafx.h"
#include "GameInstance.h"
#include "Flame_Tower.h"
#include "Monster.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "UIDamage_Manager.h"
#include "Utils.h"
#include <future>

#include "Game_Manager.h"
#include "UI_Manager.h"
#include <MonsterProjectile.h>


#include "State_DefenceTower_Idle.h"
#include "State_DefenceTower_Dead.h"
#include "State_FlameTower_Attack.h"
#include "State_DefenceTower_Prepare.h"

#include "TowerDefence_Manager.h"

USING(Client)
CFlame_Tower::CFlame_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CDefence_Tower(pDevice, pContext, strObjectTag)
{

}

CFlame_Tower::CFlame_Tower(const CFlame_Tower& rhs)
	: CDefence_Tower(rhs)
{	

}

HRESULT CFlame_Tower::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlame_Tower::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_eTowerType = CTowerDefence_Manager::TOWER_TYPE::FLAME;

	m_fFireTime = 15.f;
	return S_OK;
}

void CFlame_Tower::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pStateCom->Tick_State(fTimeDelta);

	if (nullptr != m_pTarget)
	{
		if (DEFENCE_TOWER_STATE::TOWER_STATE_IDLE == m_pStateCom->Get_CurrState())
		{
			m_fAccFireTime += fTimeDelta;
			if (m_fAccFireTime >= m_fFireTime)
			{
				m_pStateCom->Change_State(DEFENCE_TOWER_STATE::TOWER_STATE_ATTACK);
				m_fAccFireTime = 0.f;
			}
		}
	}
	else
	{
		m_fAccFireTime = m_fFireTime;
	}
}




void CFlame_Tower::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CFlame_Tower::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlame_Tower::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;
	return S_OK;
}

void CFlame_Tower::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CFlame_Tower::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CFlame_Tower::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CFlame_Tower::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CFlame_Tower::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CFlame_Tower::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}


HRESULT CFlame_Tower::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Flame_Tower_Barrel"), TEXT("Com_Model"), (CComponent**)&m_pBarrelModelCom)))
		return E_FAIL;

	/* For.Com_BaseModel */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Flame_Tower_Base"), TEXT("Com_TowerBaseModel"), (CComponent**)&m_pBaseModelCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	m_pBarrelModelCom->Set_Animation(0);
	m_pBaseModelCom->Set_Animation(0);

	m_pRigidBodyCom->Set_Use_Gravity(false);
	m_pRigidBodyCom->Set_Ground(true);

	return S_OK;
}

HRESULT CFlame_Tower::Ready_States()
{
	list<wstring> strAnimationNames;


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FlareTower.ao|FlareTower_Stand");

	if (FAILED(m_pStateCom->Add_State(DEFENCE_TOWER_STATE::TOWER_STATE_PREPARE, CState_DefenceTower_Prepare::Create(m_pStateCom, strAnimationNames))))
		return E_FAIL;


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FlareTower.ao|FlareTower_Stand");

	if (FAILED(m_pStateCom->Add_State(DEFENCE_TOWER_STATE::TOWER_STATE_IDLE, CState_DefenceTower_Idle::Create(m_pStateCom, strAnimationNames))))
		return E_FAIL;


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FlareTower.ao|FlareTower_Action");

	if (FAILED(m_pStateCom->Add_State(DEFENCE_TOWER_STATE::TOWER_STATE_ATTACK, CState_FlameTower_Attack::Create(m_pStateCom, strAnimationNames))))
		return E_FAIL;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FlareTower.ao|FlareTower_Stand");

	if (FAILED(m_pStateCom->Add_State(DEFENCE_TOWER_STATE::TOWER_STATE_DEAD, CState_DefenceTower_Dead::Create(m_pStateCom, strAnimationNames))))
		return E_FAIL;

	m_pStateCom->Change_State(DEFENCE_TOWER_STATE::TOWER_STATE_IDLE);

	return S_OK;
}

HRESULT CFlame_Tower::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 10.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pBarrelModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;


	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 100.f, 100.f, 100.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pBarrelModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 100.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CFlame_Tower::Fire()
{
	m_pStateCom->Change_State(DEFENCE_TOWER_STATE::TOWER_STATE_ATTACK);
	// TODO :: ����Ʈ..?
}

CFlame_Tower* CFlame_Tower::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag)
{
	CFlame_Tower* pInstance = new CFlame_Tower(pDevice, pContext, strObjTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed. : CFlame_Tower");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CFlame_Tower::Clone(void* pArg)
{
	CFlame_Tower* pInstance = new CFlame_Tower(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Clone Failed. : CFlame_Tower");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFlame_Tower::Free()
{
	__super::Free();
}
