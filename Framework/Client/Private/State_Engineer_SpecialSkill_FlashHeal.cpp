#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_SpecialSkill_FlashHeal.h"

CState_Engineer_SpecialSkill_FlashHeal::CState_Engineer_SpecialSkill_FlashHeal(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_SpecialSkill_FlashHeal::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_SpecialSkill_FlashHeal::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_SpecialSkill_FlashHeal::Tick_State(_float fTimeDelta)
{

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_SpecialSkill_FlashHeal::Exit_State()
{
    
}

CState_Engineer_SpecialSkill_FlashHeal* CState_Engineer_SpecialSkill_FlashHeal::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_SpecialSkill_FlashHeal* pInstance = new CState_Engineer_SpecialSkill_FlashHeal(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_SpecialSkill_FlashHeal");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_SpecialSkill_FlashHeal::Free()
{
    __super::Free();
}