#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

_uint CGameObject::g_ObjecId = 0;

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, _int iObjectType)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_strObjectTag(strObjectTag)
	, m_iObjectType(iObjectType)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_strPrototypeTag(rhs.m_strPrototypeTag)
	, m_strObjectTag(rhs.m_strObjectTag)
	, m_iObjectID(g_ObjecId++)
	, m_iObjectType(rhs.m_iObjectType)
	, m_bDead(false)
	, m_bReserveDead(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	vector<CCollider*> ColliderVector;
	ColliderVector.reserve(10);
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)	
		m_Colliders.emplace(i, ColliderVector);


	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
	
}

void CGameObject::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Render_ShadowDepth()
{
	return S_OK;
}

HRESULT CGameObject::Render_Reflect()
{


	return S_OK;
}


HRESULT CGameObject::Add_Component(const wstring& strComponentTag, CComponent* pComponent)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);
	Safe_AddRef(pComponent);
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(pComponentTag))
		return E_FAIL;


	CComponent* pComponent = GI->Clone_Component(iLevelIndex, pPrototypeTag, this, pArg);
	if (nullptr == pComponent)
		return E_FAIL;


	pComponent->Set_Owner(this);
	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring& strComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(strComponentTag.c_str()));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Compute_CamZ(_fvector vWorldPos)
{

	_float4 vCamPos = GI->Get_CamPosition();
	_vector	vCamPosition = XMLoadFloat4(&vCamPos);

	m_fCamDistance = XMVectorGetX(XMVector3Length(vWorldPos - vCamPosition));

	return S_OK;

}

CTransform* CGameObject::Get_Component_Transform()
{
	return Get_Component<CTransform>(L"Com_Transform");
}

CModel* CGameObject::Get_Component_Model()
{
	return Get_Component<CModel>(L"Com_Model");
}

CStateMachine* CGameObject::Get_Component_StateMachine()
{
	return Get_Component<CStateMachine>(L"Com_StateMachine");
}

CShader* CGameObject::Get_Component_Shader()
{
	return Get_Component<CShader>(L"Com_Shader");
}

CRigidBody* CGameObject::Get_Component_Rigidbody()
{
	return Get_Component<CRigidBody>(L"Com_RigidBody");
}

CRenderer* CGameObject::Get_Component_Renderer()
{
	return Get_Component<CRenderer>(L"Com_Renderer");
}

HRESULT CGameObject::Add_Collider(_uint iLevelIndex, _uint eColliderType, _uint eDetectionType, void* pArg)
{
	CComponent* pComponent = nullptr;
	if (eColliderType == CCollider::COLLIDER_TYPE::SPHERE)
		pComponent = GI->Clone_Component(iLevelIndex, L"Prototype_Component_Sphere_Collider", this, pArg);
	else if (eColliderType == CCollider::COLLIDER_TYPE::AABB)
		pComponent = GI->Clone_Component(iLevelIndex, L"Prototype_Component_AABB_Collider", this, pArg);
	else if (eColliderType == CCollider::COLLIDER_TYPE::OBB)
		pComponent = GI->Clone_Component(iLevelIndex, L"Prototype_Component_OBB_Collider", this, pArg);
	else
		return E_FAIL;



	if (nullptr == pComponent)
		return E_FAIL;

	auto iter = m_Colliders.find(eDetectionType);
	if (iter == m_Colliders.end())
	{
		Safe_Release(pComponent);
		return E_FAIL;
	}
		
	CCollider* pCollider = dynamic_cast<CCollider*>(pComponent);
	if (nullptr == pCollider)
	{
		Safe_Release(pComponent);
		return E_FAIL;
	}

	pCollider->Set_DetectionType(CCollider::DETECTION_TYPE(eDetectionType));
	if (CCollider::DETECTION_TYPE::ATTACK == eDetectionType)
		pCollider->Set_Active(false);

	iter->second.push_back(pCollider);
	return S_OK;
}

HRESULT CGameObject::Set_ActiveColliders(_uint eDetectionType, _bool bActive)
{
	auto iter = m_Colliders.find(eDetectionType);
	if (iter == m_Colliders.end())
		return E_FAIL;

	for (auto& pCollider : iter->second)	
		pCollider->Set_Active(bActive);

	return S_OK;
}

HRESULT CGameObject::Set_Collider_AttackMode(_uint eAttackMode, _float fAirBornPower, _float fPushPower, _float fDamage, _bool bHitLag)
{
	auto iter = m_Colliders.find(CCollider::DETECTION_TYPE::ATTACK);
	if (iter == m_Colliders.end())
		return E_FAIL;

	for (auto& pCollider : iter->second)
	{
		pCollider->Set_AttackType(CCollider::ATTACK_TYPE(eAttackMode));
		pCollider->Set_HitLag(bHitLag);
		pCollider->Set_AirBorn_Power(fAirBornPower);
		pCollider->Set_PushPower(fPushPower);
		pCollider->Set_Damage(fDamage);
	}
		

	return S_OK;
}

HRESULT CGameObject::Set_Collider_Elemental(ELEMENTAL_TYPE eElementalType)
{
	auto iter = m_Colliders.find(CCollider::DETECTION_TYPE::ATTACK);
	if (iter == m_Colliders.end())
		return S_OK;

	for (auto& pCollider : iter->second)
	{
		pCollider->Set_ElementalType(eElementalType);
	}
	return S_OK;
}


void CGameObject::LateUpdate_Collider(_float fTimedelta)
{
	for (auto& Pair : m_Colliders)
	{
		for (auto& pCollider : Pair.second)
			pCollider->LateTick_Collider(fTimedelta);
	}
}
#ifdef _DEBUG

void CGameObject::Render_Collider()
{
	for (auto& Collider : m_Colliders)
	{
		for (auto& pCollider : Collider.second)
			pCollider->Render();
	}
}
#endif // DEBUG

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	m_pDevice = nullptr;
	Safe_Release(m_pContext);
	m_pContext = nullptr;

	for (auto& Pair : m_Components)	
		Safe_Release(Pair.second);

	m_Components.clear();

	for (auto& Pair : m_Colliders)
	{
		for (auto& pCollider : Pair.second)
			Safe_Release(pCollider);
		Pair.second.clear();
	}
	m_Colliders.clear();
}
