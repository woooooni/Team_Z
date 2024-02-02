#include "stdafx.h"
#include "MainQuestNode_FinalBattle08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

#include "DreamMazeWitch_Npc.h"

CMainQuestNode_FinalBattle08::CMainQuestNode_FinalBattle08()
{
}

HRESULT CMainQuestNode_FinalBattle08::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/08.MainQuest_FinalBattle/MainQuest_FinalBattle08.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_FinalBattle08::Start()
{
	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

	if (m_pWitch != nullptr)
	{
		m_pWitch->Get_Component_Model()->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Death"));
	}
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (!m_bIsTalk)
		{
			/* ��ȭ */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 1);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
			m_bIsTalk = true;
		}

		else
		{
			m_fTime += fTimeDelta;

			if (m_fTime >= 3.f)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);
				m_bIsWitchEscape = true;
			}
		}

		if (m_bIsWitchEscape)
		{
			if (m_pWitch->Get_Component_Model()->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Death") &&
				m_pWitch->Get_Component_Model()->Is_Finish() && !m_pWitch->Get_Component_Model()->Is_Tween())
			{
				m_bIsClear = true;

				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle08::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_FinalBattle08::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_05_00_KuuSay_Happy1.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pWitch->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	}
}

CMainQuestNode_FinalBattle08* CMainQuestNode_FinalBattle08::Create()
{
	CMainQuestNode_FinalBattle08* pInstance = new CMainQuestNode_FinalBattle08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle08::Free()
{
	__super::Free();
}