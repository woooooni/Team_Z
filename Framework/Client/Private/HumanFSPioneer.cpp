#include "stdafx.h"
#include "HumanFSPioneer.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

#include "UI_World_NPCTag.h"

CHumanFSPioneer::CHumanFSPioneer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFSPioneer::CHumanFSPioneer(const CHumanFSPioneer& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFSPioneer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFSPioneer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_HumanFSPioneer.ao|HumanFSPioneer_NeutralStand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 2.f, true);

	return S_OK;
}

void CHumanFSPioneer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanFSPioneer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CHumanFSPioneer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CHumanFSPioneer::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFSPioneer::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFSPioneer::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFSPioneer::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CHumanFSPioneer::Ready_States()
{
	m_strKorName = TEXT("��� ����");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFSPioneer.ao|HumanFSPioneer_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFSPioneer.ao|HumanFS03_Talk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"KM_HumanFSPioneer.ao|HumanFS03_NeutralWalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFSPioneer.ao|HumanFS03_BattleRun");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanFSPioneer::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFSPioneer::Ready_Sockets()
{
	return S_OK;
}

CHumanFSPioneer* CHumanFSPioneer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFSPioneer* pInstance = new CHumanFSPioneer(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFSPioneer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFSPioneer::Clone(void* pArg)
{
	CHumanFSPioneer* pInstance = new CHumanFSPioneer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFSPioneer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFSPioneer::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}