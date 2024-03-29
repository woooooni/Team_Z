#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_AbNormality_Stun.h"

CState_Character_AbNormality_Stun::CState_Character_AbNormality_Stun(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_AbNormality_Stun::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_Character_AbNormality_Stun::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Set_Infinite(999.f, true);
}

void CState_Character_AbNormality_Stun::Tick_State(_float fTimeDelta)
{
    m_fAccRecovery += fTimeDelta;
    if (m_fAccRecovery >= m_fRecoveryTime)
    {
        m_fAccRecovery = 0.f;
        m_pStateMachineCom->Change_State(CCharacter::BATTLE_IDLE);
    }
}

void CState_Character_AbNormality_Stun::Exit_State()
{
    m_fAccRecovery = 0.f;
    
    m_pCharacter->Set_Infinite(1.f, true);
}


CState_Character_AbNormality_Stun* CState_Character_AbNormality_Stun::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_AbNormality_Stun* pInstance = new CState_Character_AbNormality_Stun(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_AbNormality_Stun");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_AbNormality_Stun::Free()
{
    __super::Free();
}
