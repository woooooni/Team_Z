#include "stdafx.h"
#include "GameInstance.h"
#include "Enemy_Biplane_BulletBall.h"

#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Vehicle_Flying.h"
#include "Character.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Enemy_Biplane_Bullet.h"


CEnemy_Biplane_BulletBall::CEnemy_Biplane_BulletBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVehicleFlying_Projectile(pDevice, pContext, L"Enemy_Biplane_BulletBall", OBJ_TYPE::OBJ_GRANDPRIX_ENEMY_PROJECTILE)
{

}

CEnemy_Biplane_BulletBall::CEnemy_Biplane_BulletBall(const CEnemy_Biplane_BulletBall& rhs)
	: CVehicleFlying_Projectile(rhs)
{
}


HRESULT CEnemy_Biplane_BulletBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy_Biplane_BulletBall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	m_pTargetTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform();

	if (nullptr == m_pTargetTransform)
		return E_FAIL;

	return S_OK;
}

void CEnemy_Biplane_BulletBall::Tick(_float fTimeDelta)
{
	
	__super::Tick(fTimeDelta);

	if (true == m_bMove)
	{
		m_fAccMove += fTimeDelta;
		if (m_fAccMove >= m_fMoveTime)
		{
			m_bMove = false;
			return;
		}

		Vec3 vDir = m_pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
		if (vDir.Length() > 1.f)
			m_pTransformCom->Move(XMVector3Normalize(vDir), 14.f, fTimeDelta);
	}
	else
	{
		m_fAccFireBullet += fTimeDelta;
		if (m_fAccFireBullet >= m_fFireBulletTime)
		{
			m_fAccFireBullet = 0.f;
			Fire_Bullet();
		}
	}

	Update_Rotaion(fTimeDelta);
	
}

void CEnemy_Biplane_BulletBall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEnemy_Biplane_BulletBall::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CEnemy_Biplane_BulletBall::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bullet_Orange"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CEnemy_Biplane_BulletBall::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER) && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		
	}
}

void CEnemy_Biplane_BulletBall::Update_Rotaion(_float fTimeDelta)
{
	//	fRoll += XMConvertToRadians(45.f * fTimeDelta);
//	fPitch += XMConvertToRadians(45.f * fTimeDelta);
//	fYaw += XMConvertToRadians(45.f * fTimeDelta);

	m_vRotationAngle.x += XMConvertToRadians(45.f * fTimeDelta);
	m_vRotationAngle.y += XMConvertToRadians(45.f * fTimeDelta);
	m_vRotationAngle.z += XMConvertToRadians(45.f * fTimeDelta);

	// 각 축에 대한 쿼터니언 생성
	Vec4 vRoll = XMQuaternionRotationRollPitchYaw(m_vRotationAngle.x, 0.0f, 0.0f);
	Vec4 vPitch = XMQuaternionRotationRollPitchYaw(0.0f, m_vRotationAngle.y, 0.0f);
	Vec4 vYaw = XMQuaternionRotationRollPitchYaw(0.0f, 0.0f, m_vRotationAngle.z);

	// 각 축의 쿼터니언을 곱하여 최종 회전 쿼터니언 얻음
	Vec4 vQuaternion = XMQuaternionMultiply(vYaw, XMQuaternionMultiply(vPitch, vRoll));

	// 쿼터니언을 행렬로 변환
	_matrix rotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	Vec3 vScale = m_pTransformCom->Get_Scale();

	// 행렬을 상태에 적용
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, rotationMatrix.r[0] * vScale.x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, rotationMatrix.r[1] * vScale.y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, rotationMatrix.r[2] * vScale.z);
}

void CEnemy_Biplane_BulletBall::Fire_Bullet()
{
	CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
	ProjectileDesc.pOwner = m_pOwner;

	CEnemy_Biplane_Bullet* pBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

	if (nullptr == pBullet)
	{
		pBullet = dynamic_cast<CEnemy_Biplane_Bullet*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));
	}


	if (nullptr == pBullet)
		return;

	pBullet->Set_Owner(m_pOwner);

	CTransform* pTransform = pBullet->Get_Component<CTransform>(L"Com_Transform");
	Vec3 vScale = pTransform->Get_Scale();
	
	pTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	pTransform->Set_Scale(vScale);


	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pBullet)))
		MSG_BOX("Generate Bullet Failed.");
}


CEnemy_Biplane_BulletBall* CEnemy_Biplane_BulletBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnemy_Biplane_BulletBall* pInstance = new CEnemy_Biplane_BulletBall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CEnemy_Biplane_BulletBall");
		Safe_Release<CEnemy_Biplane_BulletBall*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEnemy_Biplane_BulletBall::Clone(void* pArg)
{
	CEnemy_Biplane_BulletBall* pInstance = new CEnemy_Biplane_BulletBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CEnemy_Biplane_BulletBall");
		Safe_Release<CEnemy_Biplane_BulletBall*>(pInstance);
	}

	return pInstance;
}

void CEnemy_Biplane_BulletBall::Free()
{
	__super::Free();
}
