#include "stdafx.h"
#include "Clown.h"

#include "GameInstance.h"

#include "ClownBT.h"

CClown::CClown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CClown::CClown(const CClown& rhs)
	: CMonster(rhs)
{
}

HRESULT CClown::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CClown::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CClown::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::NUM_7))
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK];
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];
	}
	if (KEY_TAP(KEY::I) || KEY_TAP(KEY::O))
	{
		m_tStat.fHp -= 10.f;
	}

	// << : Test 
	if (KEY_TAP(KEY::HOME))
	{
		_uint iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex();
		m_pModelCom->Set_Animation(iCurAnimIndex + 1);
	}
	else if (KEY_TAP(KEY::DEL))
	{
		_int iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex() - 1;
		if (iCurAnimIndex < 0)
			iCurAnimIndex = 0;
		m_pModelCom->Set_Animation(iCurAnimIndex);
	}
	// >> 

	__super::Tick(fTimeDelta);
}

void CClown::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CClown::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CClown::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CClown::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CClown::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CClown::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CClown::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-2.f, 1.f, -6.f, 1.f));
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	/* 로밍 경로(임시) */
	m_vecRoamingArea.push_back(XMVectorSet(4.f, 1.f, -2.f, 1.f));
	m_vecRoamingArea.push_back(XMVectorSet(-5.f, 1.f, -3.f, 1.f));
	m_vecRoamingArea.push_back(m_vOriginPos);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Clown"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	//CRigidBody::RIGID_BODY_DESC RigidDesc;
	//RigidDesc.pNavigation = m_pNavigationCom;
	//RigidDesc.pTransform = m_pTransformCom;
	//
	//
	//RigidDesc.PhysXDesc.vOffsetPos = { 0.f, 0.f, 0.f };
	//RigidDesc.PhysXDesc.vExtents = { 5.f, 5.f, 10.f };
	//
	//RigidDesc.PhysXDesc.eColliderType = PHYSX_COLLIDER_TYPE::BOX;
	//RigidDesc.PhysXDesc.eRigidType = PHYSX_RIGID_TYPE::DYNAMIC;
	//
	//RigidDesc.PhysXDesc.bLockAngle_X = true;
	//RigidDesc.PhysXDesc.bLockAngle_Y = false;
	//RigidDesc.PhysXDesc.bLockAngle_Z = true;
	//
	//RigidDesc.PhysXDesc.bKinematic = false;
	//RigidDesc.PhysXDesc.fAngularDamping = 30.f;
	//RigidDesc.PhysXDesc.fDensity = 1.f;
	//
	//
	//RigidDesc.PhysXDesc.fStaticFriction = 0.f;
	//RigidDesc.PhysXDesc.fDynamicFriction = 1.f;
	//RigidDesc.PhysXDesc.fRestitution = 0.f;
	//
	//RigidDesc.PhysXDesc.fMaxVelocity = 10.f;
	//RigidDesc.PhysXDesc.pGameObject = this;
	//RigidDesc.PhysXDesc.bKinematic = true;
	//
	///* For. Com_RigidBody*/
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CClown::Ready_States()
{
	m_pBTCom = CClownBT::Create(m_pDevice, m_pContext, this);

	m_tStat.fMaxHp = 100;
	m_tStat.fHp = 100;

	return S_OK;
}

HRESULT CClown::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.2f;
	ColliderDesc.tSphere = tSphere;

	ColliderDesc.pOwner = this;
	ColliderDesc.pNode = nullptr;
	ColliderDesc.pOwnerTransform = m_pTransformCom;
	ColliderDesc.ModePivotMatrix = m_pModelCom->Get_PivotMatrix();
	ColliderDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);
	ColliderDesc.bLockAngle_X = false;
	ColliderDesc.bLockAngle_Y = false;
	ColliderDesc.bLockAngle_Z = false;

	ColliderDesc.fAngularDamping = 0.f;
	ColliderDesc.fDensity = 1.f;

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

CClown* CClown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CClown* pInstance = new CClown(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CClown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CClown::Clone(void* pArg)
{
	CClown* pInstance = new CClown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CClown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown::Free()
{
	__super::Free();

}
