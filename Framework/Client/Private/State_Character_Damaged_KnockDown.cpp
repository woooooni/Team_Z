#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Damaged_KnockDown.h"

CState_Character_Damaged_KnockDown::CState_Character_Damaged_KnockDown(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Damaged_KnockDown::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Damaged_KnockDown::Enter_State(void* pArg)
{
    CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
    wstring strSoundKey = L"";
    switch (eCharacterType)
    {
    case CHARACTER_TYPE::SWORD_MAN:
        strSoundKey = L"Swordsman_V_Dmg_Long_" + to_wstring(GI->RandomInt(1, 3));
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::ENGINEER:
        strSoundKey = L"Engineer_V_Dmg_Long_" + to_wstring(GI->RandomInt(1, 3));
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::DESTROYER:
        strSoundKey = L"Destroyer_V_Dmg_Long_" + to_wstring(GI->RandomInt(1, 3));
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    }

    m_iCurrAnimIndex = m_AnimIndices[0];
    m_fAccRecovery = 0.f;
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Character_Damaged_KnockDown::Tick_State(_float fTimeDelta)
{
    if (m_iCurrAnimIndex == m_AnimIndices[0])
    {
        if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        {
            m_iCurrAnimIndex = m_AnimIndices[1];
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
            return;
        }
    }
    else if (m_iCurrAnimIndex == m_AnimIndices[1])
    {
        m_fAccRecovery += fTimeDelta;
        if (m_fAccRecovery >= m_fRecoveryTime)
        {
            m_fAccRecovery = 0.f;
            m_iCurrAnimIndex = m_AnimIndices[2];
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
            return;
        }
    }
    else if (m_iCurrAnimIndex == m_AnimIndices[2])
    {
        if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
            return;
        }

    }
}

void CState_Character_Damaged_KnockDown::Exit_State()
{
    m_iCurrAnimIndex = 0;
    m_fAccRecovery = 0.f;
    
}

CState_Character_Damaged_KnockDown* CState_Character_Damaged_KnockDown::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Damaged_KnockDown* pInstance = new CState_Character_Damaged_KnockDown(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Damaged_KnockDown::Free()
{
    __super::Free();
}
