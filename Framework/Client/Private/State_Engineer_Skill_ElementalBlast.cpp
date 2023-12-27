#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Engineer_Skill_ElementalBlast.h"

CState_Engineer_Skill_ElementalBlast::CState_Engineer_Skill_ElementalBlast(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_Skill_ElementalBlast::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_Skill_ElementalBlast::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Engineer_Skill_ElementalBlast::Tick_State(_float fTimeDelta)
{

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_Skill_ElementalBlast::Exit_State()
{
    
}


CState_Engineer_Skill_ElementalBlast* CState_Engineer_Skill_ElementalBlast::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_Skill_ElementalBlast* pInstance = new CState_Engineer_Skill_ElementalBlast(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_Skill_ElementalBlast::Free()
{
    __super::Free();
}