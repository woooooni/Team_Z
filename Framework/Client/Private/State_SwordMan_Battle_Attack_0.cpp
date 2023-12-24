#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Attack_0.h"

CState_SwordMan_Battle_Attack_0::CState_SwordMan_Battle_Attack_0(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Attack_0::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Battle_Attack_0::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Battle_Attack_0::Tick_State(_float fTimeDelta)
{
    Input(fTimeDelta);

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Battle_Attack_0::Exit_State()
{
    
}

void CState_SwordMan_Battle_Attack_0::Input(_float fTimeDelta)
{
    if (KEY_TAP(KEY::LBTN))
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_ATTACK_1);
}

CState_SwordMan_Battle_Attack_0* CState_SwordMan_Battle_Attack_0::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Attack_0* pInstance = new CState_SwordMan_Battle_Attack_0(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Attack_0");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Attack_0::Free()
{
    __super::Free();
}