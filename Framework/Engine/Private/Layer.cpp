#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);
	return S_OK;
}

void CLayer::Priority_Tick(_float fTimeDelta)
{
	auto iter = m_GameObjects.begin();

	while (iter != m_GameObjects.end())
	{
		if ((*iter) == nullptr)
		{
			iter = m_GameObjects.erase(iter);
		}

		else if ((*iter)->Is_Dead())
		{
			Safe_Release((*iter));
			iter = m_GameObjects.erase(iter);
		}

		else
		{
			(*iter)->Priority_Tick(fTimeDelta * (*iter)->Get_ObjectTimeScale());
			iter++;
		}
	}
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Tick(fTimeDelta * pGameObject->Get_ObjectTimeScale());
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->LateTick(fTimeDelta * pGameObject->Get_ObjectTimeScale());
	}
}

void CLayer::Clear()
{
	for (auto& iter : m_GameObjects)
		Safe_Release(iter);

	m_GameObjects.clear();
}

CGameObject* CLayer::Find_GameObject(const wstring& strObjectTag)
{
	CGameObject* pObj = nullptr;
	auto iter = find_if(m_GameObjects.begin(), m_GameObjects.end(), [&](CGameObject* pObj) 
	{
		return pObj->Get_ObjectTag() == strObjectTag;
	});

	if (iter == m_GameObjects.end())
		return nullptr;

	return *iter;
}

CGameObject* CLayer::Find_GameObject(_int iObjectID)
{
	CGameObject* pObj = nullptr;
	auto iter = find_if(m_GameObjects.begin(), m_GameObjects.end(), [&](CGameObject* pObj)
		{
			return pObj->Get_ObjectID() == iObjectID;
		});

	if (iter == m_GameObjects.end())
		return nullptr;

	return *iter;
}


CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
