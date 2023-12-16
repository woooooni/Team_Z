#include "stdafx.h"
#include "Stellia.h"

#include "GameInstance.h"

#include "StelliaBT.h"

CStellia::CStellia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CStellia::CStellia(const CStellia& rhs)
	: CMonster(rhs)
{
}

HRESULT CStellia::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStellia::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;


	//m_pModelCom->Set_Animation(GI->RandomInt(0, 10));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 1.f, 1.f, 1.f));


	return S_OK;
}

void CStellia::Tick(_float fTimeDelta)
{
	// << : Test 
	if (KEY_TAP(KEY::INSERT))
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

	if (GetAsyncKeyState('G'))
	{
		m_tStat.fHp = 0;
	}
}

void CStellia::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CStellia::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStellia::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CStellia::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CStellia::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CStellia::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CStellia::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(GI->RandomFloat(-100.f, 100.f), GI->RandomFloat(-5.f, 5.f), GI->RandomFloat(-100.f, 100.f), 1.f));
	
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stellia"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pNavigation = m_pNavigationCom;
	RigidDesc.pTransform = m_pTransformCom;
	
	
	RigidDesc.PhysXDesc.vOffsetPos = { 0.f, 0.f, 0.f };
	RigidDesc.PhysXDesc.vExtents = { 5.f, 5.f, 10.f };
	
	RigidDesc.PhysXDesc.eColliderType = PHYSX_COLLIDER_TYPE::BOX;
	RigidDesc.PhysXDesc.eRigidType = PHYSX_RIGID_TYPE::DYNAMIC;
	
	RigidDesc.PhysXDesc.bLockAngle_X = true;
	RigidDesc.PhysXDesc.bLockAngle_Y = false;
	RigidDesc.PhysXDesc.bLockAngle_Z = true;
	
	RigidDesc.PhysXDesc.bKinematic = false;
	RigidDesc.PhysXDesc.fAngularDamping = 30.f;
	RigidDesc.PhysXDesc.fDensity = 1.f;
	
	
	RigidDesc.PhysXDesc.fStaticFriction = 0.f;
	RigidDesc.PhysXDesc.fDynamicFriction = 1.f;
	RigidDesc.PhysXDesc.fRestitution = 0.f;
	
	RigidDesc.PhysXDesc.fMaxVelocity = 10.f;
	RigidDesc.PhysXDesc.pGameObject = this;
	RigidDesc.PhysXDesc.bKinematic = true;
	
	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStellia::Ready_States()
{
	m_pBTCom = CStelliaBT::Create(m_pDevice, m_pContext, this);

	m_tStat.fMaxHp = 199;
	m_tStat.fHp = 199;

	return S_OK;
}

HRESULT CStellia::Ready_Colliders()
{
	return S_OK;
}

CStellia* CStellia::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CStellia* pInstance = new CStellia(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CStellia");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStellia::Clone(void* pArg)
{
	CStellia* pInstance = new CStellia(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStellia");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia::Free()
{
	__super::Free();

}
