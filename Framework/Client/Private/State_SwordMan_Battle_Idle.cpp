#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Idle.h"

CState_SwordMan_Battle_Idle::CState_SwordMan_Battle_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Battle_Idle::Enter_State(void* pArg)
{
    m_fAccReturnNuetral = 0.f;
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Battle_Idle::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);

    m_fAccReturnNuetral += fTimeDelta;
    if (m_fAccReturnNuetral >= m_fReturnNuetralTime)
    {
        m_fAccReturnNuetral = 0.f;
        m_pModelCom->Set_Animation(m_AnimIndices[1]);
    }
}

void CState_SwordMan_Battle_Idle::Exit_State()
{
    m_fAccReturnNuetral = 0.f;
}

void CState_SwordMan_Battle_Idle::Input(_float fTimeDelta)
{
    if (KEY_HOLD(KEY::W) || KEY_TAP(KEY::A) || KEY_TAP(KEY::S) || KEY_TAP(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_WALK);
    
    if (KEY_TAP(KEY::SPACE))
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_JUMP);
}

CState_SwordMan_Battle_Idle* CState_SwordMan_Battle_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Idle* pInstance = new CState_SwordMan_Battle_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Idle::Free()
{
    __super::Free();
}
