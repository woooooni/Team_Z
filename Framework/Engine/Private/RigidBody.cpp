#include "RigidBody.h"
#include "GameInstance.h"
#include "GameObject.h"

CRigidBody::CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
	
}

CRigidBody::CRigidBody(CRigidBody& rhs)
	: CComponent(rhs)
{

}

HRESULT CRigidBody::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CRigidBody::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	RIGID_BODY_DESC* pDesc = static_cast<RIGID_BODY_DESC*>(pArg);
	m_pTransformCom = pDesc->pTransform;
	
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	Safe_AddRef(m_pTransformCom);


	return S_OK;
}


void CRigidBody::Update_RigidBody(_float fTimeDelta)
{
	if (m_pOwner->Is_Dead())
		return;

	Update_Gravity(fTimeDelta);
	Update_Velocity(fTimeDelta);
}



void CRigidBody::Update_Gravity(_float fTimeDelta)
{
	if (false == m_bUseGravity || true == m_bGround)
		return;

	m_vVelocity += Vec3(0.f, -9.8f, 0.f) * fTimeDelta;
}

void CRigidBody::Update_Velocity(_float fTimeDelta)
{
	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	if (0.f < m_vVelocity.Length())
	{
		WorldMatrix.r[CTransform::STATE_POSITION] += m_vVelocity * fTimeDelta;

		Vec3 vVelocity = m_vVelocity;
		vVelocity.y = 0.f;

		Vec3 vFriction = -1.f * vVelocity * m_fFrictionScale * fTimeDelta;


		if (vFriction.Length() >= m_vVelocity.Length())
		{
			m_vVelocity = Vec3(0.f, 0.f, 0.f);
		}
		else
		{
			if (fabs(vVelocity.x) < fabs(vFriction.x))
				vFriction.x = vVelocity.x;

			if (fabs(vVelocity.y) < fabs(vFriction.y))
				vFriction.y = vVelocity.y;

			if (fabs(vVelocity.z) < fabs(vFriction.z))
				vFriction.z = vVelocity.z;
			m_vVelocity += vFriction;
		}
	}

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
}


void CRigidBody::Add_Velocity(_vector vDir, _float fForce, _bool bClear)
{
	if (true == bClear)
		m_vVelocity = { 0.f, 0.f, 0.f };

	m_vVelocity += XMVector3Normalize(vDir) * fForce;
}

const _bool& CRigidBody::Check_Sleep()
{
	m_bSleep = (m_vVelocity.Length() < m_fSleepThreshold) ? true : false; 
	
	if(m_bSleep)
		ZeroMemory(&m_vVelocity, sizeof(Vec3));
	
	return m_bSleep;
}

CRigidBody* CRigidBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRigidBody* pInstance = new CRigidBody(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Crate Failed. : CRigidBody");
		return nullptr;
	}
	return pInstance;
}

CComponent* CRigidBody::Clone(void* pArg)
{
	CRigidBody* pInstance = new CRigidBody(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CRigidBody");
		return nullptr;
	}

	return pInstance;
}

void CRigidBody::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
}

