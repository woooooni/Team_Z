#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Pick_Small_Enter.h"

CState_Character_Neutral_Pick_Small_Enter::CState_Character_Neutral_Pick_Small_Enter(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Pick_Small_Enter::Initialize(const list<wstring>& AnimationList)
{
    
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_Character_Neutral_Pick_Small_Enter::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    
    if(nullptr == m_pCharacter->Get_Target())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
}

void CState_Character_Neutral_Pick_Small_Enter::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_SMALL_IDLE);

    if (nullptr != m_pCharacter->Get_Target())
    {
        CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr != pTargetTransform)
        {
            pTargetTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());

            Vec4 vHandCenterPosition = {};
            Vec4 vLeftHandPosition = (m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix()).Translation();
            Vec4 vRightHandPosition = (m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix()).Translation();

            vHandCenterPosition = XMVectorSetW((vLeftHandPosition + vRightHandPosition) / 2.f, 1.f);
            vHandCenterPosition.y -= 0.1f;
            pTargetTransform->Set_State(CTransform::STATE_POSITION, vHandCenterPosition);
        }
    }
    else
    {
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
    }
}

void CState_Character_Neutral_Pick_Small_Enter::Exit_State()
{

}


CState_Character_Neutral_Pick_Small_Enter* CState_Character_Neutral_Pick_Small_Enter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Pick_Small_Enter* pInstance = new CState_Character_Neutral_Pick_Small_Enter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Pick_Small_Enter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Pick_Small_Enter::Free()
{
    __super::Free();
}
