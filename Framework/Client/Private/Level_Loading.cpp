#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"


#include "Level_Tool.h"
#include "Level_Test.h"
#include "ImGui_Manager.h"


CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}


HRESULT CLevel_Loading::Initialize(LEVELID eNextLevel, const wstring& strFolderName)
{
	m_eNextLevel = eNextLevel;

	if (m_eNextLevel == LEVEL_TOOL)
	{
		CImGui_Manager::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);
	}


	/* m_eNextLevel 에 대한 로딩작업을 수행한다. */
	/* 로딩을 겁나 하고있다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel, strFolderName);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Loading::LateTick(_float fTimeDelta)
{
	wstring		strLoadingText = m_pLoader->Get_LoadingText();

	SetWindowText(g_hWnd, strLoadingText.c_str());

	if (true == m_pLoader->Get_Finished())
	{
		if (KEY_TAP(KEY::SPACE))
		{
			CLevel* pNewLevel = nullptr;

			switch (m_eNextLevel)
			{

			case LEVEL_TOOL:
				pNewLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
				break;

			case LEVEL_TEST:
				pNewLevel = CLevel_Test::Create(m_pDevice, m_pContext);
				break;


			}

			if (nullptr == pNewLevel)
				return E_FAIL;

			if (FAILED(GI->Open_Level(m_eNextLevel, pNewLevel)))
				return E_FAIL;
		}

	}


	return S_OK;
}

HRESULT CLevel_Loading::Enter_Level()
{
	return S_OK;
}

HRESULT CLevel_Loading::Exit_Level()
{
	return S_OK;
}


CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel, const wstring& strFolderName)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel, strFolderName)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pLoader);
}
