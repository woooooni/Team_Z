#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Dummy_Destroyer.h"
#include "State_Lobby_Destroyer_Sit.h"

CState_Lobby_Destroyer_Sit::CState_Lobby_Destroyer_Sit(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Lobby_Destroyer_Sit::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;

    m_pDestroyer = dynamic_cast<CUI_Dummy_Destroyer*>(m_pCharacter);

    return S_OK;
}

void CState_Lobby_Destroyer_Sit::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Lobby_Destroyer_Sit::Tick_State(_float fTimeDelta)
{
    if (m_pDestroyer->Is_Clicked())
    {
        if (m_iCurrAnimIndex != m_AnimIndices[4])
        {
            m_iCurrAnimIndex = m_AnimIndices[4];

            GI->Stop_Sound(CHANNELID::SOUND_VOICE_CHARACTER);

            switch (GI->RandomInt(0, 1))
            {
            case 0:
                GI->Play_Sound(TEXT("Destroyer_System_Character_Choice_1.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
                break;

            case 1:
                GI->Play_Sound(TEXT("Destroyer_System_Character_Choice_2.mp3"), CHANNELID::SOUND_VOICE_CHARACTER, GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_CHARACTER));
                break;
            }

            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
        }

        if (m_iCurrAnimIndex == m_AnimIndices[4] &&
            false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
            return;
        }
    }
    else
    {
        if (m_iCurrAnimIndex == m_AnimIndices[0])
        {
             if (false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
             {
                 m_fSitTimeAcc = 0.f;
                 m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 3)];
                 m_pModelCom->Set_Animation(m_iCurrAnimIndex, 0.5f);
             }
        }

        if (m_iCurrAnimIndex != m_AnimIndices[0] &&
            false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
        {
            m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 3)];

            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
        }
    }
}

void CState_Lobby_Destroyer_Sit::Exit_State()
{
    m_fSitTimeAcc = 0;
    m_iCurrAnimIndex = 0;
}

CState_Lobby_Destroyer_Sit* CState_Lobby_Destroyer_Sit::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Lobby_Destroyer_Sit* pInstance = new CState_Lobby_Destroyer_Sit(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Lobby_Destroyer_Sit");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Lobby_Destroyer_Sit::Free()
{
    __super::Free();
}
