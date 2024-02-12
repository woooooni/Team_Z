#include "stdafx.h"
#include "SubQuestNode_NoisySnowField06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"
#include "CurlingGame_Group.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CSubQuestNode_NoisySnowField06::CSubQuestNode_NoisySnowField06()
{
}

HRESULT CSubQuestNode_NoisySnowField06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[서브]");
	m_strQuestName = TEXT("시끄러운 코에루코 설원");
	m_strQuestContent = TEXT("대결에서 승리하여 진정시키자");

	m_strNextQuestTag = TEXT("[서브]");
	m_strNextQuestName = TEXT("시끄러운 코에루코 설원");
	m_strNextQuestContent = TEXT("벨라에게 돌아가기");



	return S_OK;
}

void CSubQuestNode_NoisySnowField06::Start()
{
	if (CCurlingGame_Manager::GetInstance()->Is_PlayerWin())
	{
		// 플레이어가 승리했다면 
		Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField06.json");

		for (const auto& talkDesc : Load) {
			TALK_DELS sTalkDesc;
			sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
			sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
			m_vecTalkDesc.push_back(sTalkDesc);
		}

		m_bIsVictory = true;
	}
	else
	{
		// NPC가 승리 했다면
		Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField06_Fail.json");

		for (const auto& talkDesc : Load) {
			TALK_DELS sTalkDesc;
			sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
			sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
			m_vecTalkDesc.push_back(sTalkDesc);
		}

		m_bIsVictory = false;
	}

	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pDestroyer = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"Destroyer_Dummy");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pDestroyer);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

				/* 대화 카메라 종료 */
				CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
				if (nullptr != pActionCam)
					pActionCam->Finish_Action_Talk();

				m_bIsFadeOut = true;
			}

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}
		}

		if (!m_bIsFadeIn && m_bIsFadeOut && Is_EndCameraBlender())
		{
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);

			m_bIsFadeIn = true;
		}

		if (m_bIsFadeIn)
		{
			if (CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);

				m_pDestroyer->Set_Dead(true);
				m_bIsClear = true;

				return NODE_STATE::NODE_FAIL;
			}
		}

	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField06::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField06::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pDestroyer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pDestroyer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_Win"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;
	case 1:
		if (m_bIsVictory)
		{
			CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_01_Win.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
			m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
			m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHatchOut01"));
		}
		else
		{
			CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_01_Lose.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
			m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
			m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		}
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pDestroyer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pDestroyer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_03.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_NeutralStand05"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_05.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionNegative"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_06.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pDestroyer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pDestroyer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_07.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSurprise03"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_06_08.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pDestroyer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pDestroyer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_Win"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	}

}

CSubQuestNode_NoisySnowField06* CSubQuestNode_NoisySnowField06::Create()
{
	CSubQuestNode_NoisySnowField06* pInstance = new CSubQuestNode_NoisySnowField06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField06::Free()
{
	__super::Free();
}
