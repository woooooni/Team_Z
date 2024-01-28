#include "stdafx.h"
#include "MainQuestNode_Invasion02_4.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "DreamMazeWitch_Npc.h"

CMainQuestNode_Invasion02_4::CMainQuestNode_Invasion02_4()
{
}

HRESULT CMainQuestNode_Invasion02_4::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/05.MainQuest_Invasion/MainQuest_Invasion02_4.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Invasion02_4::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(nullptr);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();

}

CBTNode::NODE_STATE CMainQuestNode_Invasion02_4::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN) && m_iTalkIndex == 0)
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			m_bIsWitchAppear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			/* ��ȭ ī�޶� ���� */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();
		}

		// ��ȭ�� �ϳ��� ������ ������ �ʿ��ϱ⿡ ���⼭ ������ ���� �ɾ��ֱ�.
		if (m_iTalkIndex < 1)
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	if (m_bIsWitchAppear)
	{
		// ���� ����
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_DreamMazeWitch_Npc"), nullptr, &m_pWitch)))
		{
			MSG_BOX("Fail AddGameObj : Quest DreamerMazeWitch");
		}

		m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_INVASION_APPEAR);

		m_bIsWitchAppear = false;
	}

	if (m_pWitch != nullptr)
	{
		if (m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CDreamMazeWitch_Npc::WITCHSTATE_INVASION_IDLE)
		{
			// ���ٿͼ� ���� �ٷ� �ɸ����� Ȯ��. �ٷ� �ɸ��� �ȵǰŵ�
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion02_4::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion02_4::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_00_KuuSay_Hey~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}

}

CMainQuestNode_Invasion02_4* CMainQuestNode_Invasion02_4::Create()
{
	CMainQuestNode_Invasion02_4* pInstance = new CMainQuestNode_Invasion02_4();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion02_4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion02_4::Free()
{
	__super::Free();
}