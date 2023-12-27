#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Neutral_Pick_Small_Finish.h"

CState_Engineer_Neutral_Pick_Small_Finish::CState_Engineer_Neutral_Pick_Small_Finish(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Neutral_Pick_Small_Finish::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    return S_OK;
}

void CState_Engineer_Neutral_Pick_Small_Finish::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Neutral_Pick_Small_Finish::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
}

void CState_Engineer_Neutral_Pick_Small_Finish::Exit_State()
{

}


CState_Engineer_Neutral_Pick_Small_Finish* CState_Engineer_Neutral_Pick_Small_Finish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Neutral_Pick_Small_Finish* pInstance = new CState_Engineer_Neutral_Pick_Small_Finish(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_Neutral_Pick_Small_Finish");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Neutral_Pick_Small_Finish::Free()
{
    __super::Free();
}