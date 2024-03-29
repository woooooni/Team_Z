#include "stdafx.h"
#include "MainQuestNode_SnowField02_2.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Sound_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

CMainQuestNode_SnowField02_2::CMainQuestNode_SnowField02_2()
{
}

HRESULT CMainQuestNode_SnowField02_2::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("코에루크 설원으로");
	m_strQuestContent = TEXT("동쪽 문을 통해 설원으로 향하기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("주둔지의 지휘관 찾기");
	m_strNextQuestContent = TEXT("주둔지에서 지휘관 찾아보자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField02_2.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	m_fTalkChangeTime = 5.f;

	return S_OK;
}

void CMainQuestNode_SnowField02_2::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_SnowField02_2::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (!Is_Finish_LevelEnterCameraAction())
		return NODE_STATE::NODE_RUNNING;

	if (!m_bIsStart)
	{
		m_bIsStart = true;

		/* 대화 */
		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		// CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
		CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	if (m_bIsStart)
	{
		m_fTime += fTimeDelta;

		if (m_fTime >= m_fTalkChangeTime)
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Dialog_Text_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_PopupQuest::QUEST_INFO QuestDesc = {};
				QuestDesc.strType = m_strNextQuestTag;
				QuestDesc.strTitle = m_strNextQuestName;
				QuestDesc.strContents = m_strNextQuestContent;
				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

				m_bIsClear = true;
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

				return NODE_STATE::NODE_FAIL;
			}

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField02_2::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField02_2::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_2_00_KuuSay_DestSF.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_2_01_KuuSay_VeryCold!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive"));
		break;
	}
}

CMainQuestNode_SnowField02_2* CMainQuestNode_SnowField02_2::Create()
{
	CMainQuestNode_SnowField02_2* pInstance = new CMainQuestNode_SnowField02_2();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField02_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField02_2::Free()
{
	__super::Free();
}
