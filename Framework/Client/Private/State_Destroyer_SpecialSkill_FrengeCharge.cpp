#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_SpecialSkill_FrengeCharge.h"

#include "Effect_Manager.h"
#include "Buff_Manager.h"

CState_Destroyer_SpecialSkill_FrengeCharge::CState_Destroyer_SpecialSkill_FrengeCharge(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_SpecialSkill_FrengeCharge::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_SpecialSkill_FrengeCharge::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Look_For_Target();
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_FrengeCharge"), m_pTransformCom->Get_WorldMatrix(), m_pCharacter);
    BUFF_DESC BuffDesc = {};

    BuffDesc.iBuffCount = 5;
    BuffDesc.fEndBuffTime = 3.f;

    CBuff_Manager::GetInstance()->Apply_Buff(CHARACTER_TYPE::DESTROYER, CBuff_Manager::BUFF_TYPE::DESTROYER_FRENGE_CHARGE, BuffDesc);

}

void CState_Destroyer_SpecialSkill_FrengeCharge::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
    __super::Attack_Input(fTimeDelta);

}

void CState_Destroyer_SpecialSkill_FrengeCharge::Exit_State()
{
    
}

CState_Destroyer_SpecialSkill_FrengeCharge* CState_Destroyer_SpecialSkill_FrengeCharge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_SpecialSkill_FrengeCharge* pInstance = new CState_Destroyer_SpecialSkill_FrengeCharge(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_SpecialSkill_FrengeCharge::Free()
{
    __super::Free();
}
