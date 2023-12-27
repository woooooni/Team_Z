#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Battle_Idle.h"

CState_Engineer_Battle_Idle::CState_Engineer_Battle_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Battle_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Battle_Idle::Enter_State(void* pArg)
{
    m_fAccReturnNuetral = 0.f;
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_Engineer_Battle_Idle::Tick_State(_float fTimeDelta)
{
    m_fAccReturnNuetral += fTimeDelta;
    if (m_fAccReturnNuetral >= m_fReturnNuetralTime)
    {
        m_fAccReturnNuetral = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[1];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    if (m_iCurrAnimIndex == m_AnimIndices[1] && false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);

    Input(fTimeDelta);
    
}

void CState_Engineer_Battle_Idle::Exit_State()
{
    m_fAccReturnNuetral = 0.f;
}

void CState_Engineer_Battle_Idle::Input(_float fTimeDelta)
{
    if (true == Skill_Input(fTimeDelta))
        return;

    if (KEY_TAP(KEY::CTRL))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
        return;
    }

    if (KEY_HOLD(KEY::RBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_GUARD);
        return;
    }

    if (KEY_TAP(KEY::LBTN))
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_0);
        return;
    }
        

    if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_WALK);
    
    if (KEY_TAP(KEY::SPACE))
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_JUMP);
}

CState_Engineer_Battle_Idle* CState_Engineer_Battle_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Battle_Idle* pInstance = new CState_Engineer_Battle_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Battle_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Battle_Idle::Free()
{
    __super::Free();
}