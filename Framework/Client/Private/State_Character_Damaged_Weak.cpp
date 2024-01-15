#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Damaged_Weak.h"

CState_Character_Damaged_Weak::CState_Character_Damaged_Weak(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Damaged_Weak::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Damaged_Weak::Enter_State(void* pArg)
{
    CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
    wstring strSoundKey = L"";
    switch (eCharacterType)
    {
    case CHARACTER_TYPE::SWORD_MAN:
        strSoundKey = L"Swordsman_V_Dmg_Short_" + to_wstring(GI->RandomInt(1, 3));
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::ENGINEER:
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::DESTROYER:
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    }

    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Damaged_Weak::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Character_Damaged_Weak::Exit_State()
{
    
}


CState_Character_Damaged_Weak* CState_Character_Damaged_Weak::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Damaged_Weak* pInstance = new CState_Character_Damaged_Weak(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Damaged_Weak::Free()
{
    __super::Free();
}
