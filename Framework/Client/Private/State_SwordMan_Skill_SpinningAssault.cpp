#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Skill_SpinningAssault.h"

CState_SwordMan_Skill_SpinningAssault::CState_SwordMan_Skill_SpinningAssault(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Skill_SpinningAssault::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Skill_SpinningAssault::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Skill_SpinningAssault::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() >= 0.25f && m_pModelCom->Get_Progress() <= 0.5f)
    {
        m_pTransformCom->Move(m_pTransformCom->Get_Look(), 12.5f - (m_pModelCom->Get_Progress() * 10.f), fTimeDelta);
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Skill_SpinningAssault::Exit_State()
{
    
}

CState_SwordMan_Skill_SpinningAssault* CState_SwordMan_Skill_SpinningAssault::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Skill_SpinningAssault* pInstance = new CState_SwordMan_Skill_SpinningAssault(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Skill_SpinningAssault::Free()
{
    __super::Free();
}