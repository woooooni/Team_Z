#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Dead.h"

CState_Character_Dead::CState_Character_Dead(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Dead::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Dead::Enter_State(void* pArg)
{
    CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
    wstring strSoundKey = L"";
    switch(eCharacterType)
    {
    case CHARACTER_TYPE::SWORD_MAN:
        strSoundKey = L"Swordsman_V_Death_" + to_wstring(GI->RandomInt(1, 2));
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::ENGINEER:
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::DESTROYER:
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    }
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Dead::Tick_State(_float fTimeDelta)
{
    if (KEY_TAP(KEY::R))
    {
        m_pStateMachineCom->Change_State(CCharacter::REVIVE);
        return;
    }
}

void CState_Character_Dead::Exit_State()
{

}

CState_Character_Dead* CState_Character_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Dead* pInstance = new CState_Character_Dead(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Dead");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Dead::Free()
{
    __super::Free();
}
