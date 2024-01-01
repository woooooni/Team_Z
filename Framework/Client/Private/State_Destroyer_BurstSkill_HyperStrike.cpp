#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_BurstSkill_HyperStrike.h"

CState_Destroyer_BurstSkill_HyperStrike::CState_Destroyer_BurstSkill_HyperStrike(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_BurstSkill_HyperStrike::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_BurstSkill_HyperStrike::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_BurstSkill_HyperStrike::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_BurstSkill_HyperStrike::Exit_State()
{
    
}
CState_Destroyer_BurstSkill_HyperStrike* CState_Destroyer_BurstSkill_HyperStrike::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_BurstSkill_HyperStrike* pInstance = new CState_Destroyer_BurstSkill_HyperStrike(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_BurstSkill_HyperStrike");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_BurstSkill_HyperStrike::Free()
{
    __super::Free();
}