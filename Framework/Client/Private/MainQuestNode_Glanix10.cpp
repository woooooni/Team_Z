#include "stdafx.h"
#include "MainQuestNode_Glanix10.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

CMainQuestNode_Glanix10::CMainQuestNode_Glanix10()
{
}

HRESULT CMainQuestNode_Glanix10::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("���� �罽��");
	m_strQuestContent = TEXT("�罽���� ��ȭ�ϱ�");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix10.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Glanix10::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_KINGDOMHALL, LAYER_NPC, TEXT("Ruslan"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pRuslan);

	/* ī�޶� Ÿ�� ���� */
	// CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");

	//if (nullptr != pTarget)
	//{

		// �ӽ� �ּ�
		//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		//if (nullptr != pActionCam)
		//{
		//	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);
		//	pActionCam->Start_Action_Talk(); //���� ȥ�ڸ� null
		//}


	//}

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_Glanix10::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (!m_bIsRewarding)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

				//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
				//if (nullptr != pActionCam)
				//	pActionCam->Finish_Action_Talk();

				/* ���⼭ ����Ʈ ���� �ޱ�.(����Ʈ ���� �� ������ return�ϱ�.*/
				CUI_Manager::GetInstance()->OnOff_QuestRewards(true, TEXT("�ڿ���ũ ������ ���� �ذ��ϱ�"));
				m_bIsRewarding = true;
			}

			if (!m_bIsRewarding)
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}
		}
	}

	else if (m_bIsRewarding)
	{
		if (CUI_Manager::GetInstance()->Is_QuestRewardWindowOff())
		{
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix10::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix10::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_10_00_RuslanSay_Comeback!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pRuslan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pRuslan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Ruslan.ao|Ruslan_CSShowedTrueColors01"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_10_01_KuuSay_EHem.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_KuuSay_I_No_Pet.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_10_03_RuslanSay_ItsThis....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_10_04_RuslanSay_Reward.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_10_05_KuuSay_Happy.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		break;
	case 6:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_ImKuu.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}

}

CMainQuestNode_Glanix10* CMainQuestNode_Glanix10::Create()
{
	CMainQuestNode_Glanix10* pInstance = new CMainQuestNode_Glanix10();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix10");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix10::Free()
{
	__super::Free();
}