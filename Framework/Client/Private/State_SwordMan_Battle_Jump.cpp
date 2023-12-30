#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Animation.h"
#include "State_SwordMan_Battle_Jump.h"

CState_SwordMan_Battle_Jump::CState_SwordMan_Battle_Jump(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Jump::Initialize(const list<wstring>& AnimationList)
{
    
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Battle_Jump::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();

    m_pRigidBodyCom->Set_Ground(false);
    m_pRigidBodyCom->Set_Use_Gravity(true);

    Vec4 vPosition = m_pTransformCom->Get_Position();
    vPosition.y += 0.1f;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
    Vec3 vJumpDir = XMVector3Normalize(m_pTransformCom->Get_Look());
    vJumpDir.y = 1.f;

    m_pRigidBodyCom->Add_Velocity(XMVector3Normalize(vJumpDir), 10.f, true);
    m_pModelCom->Set_Animation(m_AnimIndices[0]);


    m_bGroundChange = false;
}

void CState_SwordMan_Battle_Jump::Tick_State(_float fTimeDelta)
{
    if (true == m_pRigidBodyCom->Is_Ground())
    {
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
    }
}

void CState_SwordMan_Battle_Jump::Exit_State()
{
    m_iCurrAnimIndex = 0;
}

void CState_SwordMan_Battle_Jump::Input(_float fTimeDelta)
{

}

CState_SwordMan_Battle_Jump* CState_SwordMan_Battle_Jump::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Jump* pInstance = new CState_SwordMan_Battle_Jump(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Jump");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Jump::Free()
{
    __super::Free();
}
