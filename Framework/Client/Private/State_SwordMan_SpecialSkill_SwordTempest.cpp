#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_SpecialSkill_SwordTempest.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "Effect_Manager.h"

CState_SwordMan_SpecialSkill_SwordTempest::CState_SwordMan_SpecialSkill_SwordTempest(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_SpecialSkill_SwordTempest::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_SpecialSkill_SwordTempest::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();


    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_SwordTempest"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_SwordMan_SpecialSkill_SwordTempest::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween())
    {
        if (m_pModelCom->Get_CurrAnimationFrame() == 9)
        {
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 3.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 12)
        {
            m_pTransformCom->Move(1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 3.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 16)
        {
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 3.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 20)
        {
            m_pTransformCom->Move(1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 3.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 25)
        {
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 3.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 29)
        {
            m_pTransformCom->Move(1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 3.f, fTimeDelta);
        }

        if (m_pModelCom->Get_CurrAnimationFrame() == 33)
        {
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Look()), 10.f, fTimeDelta);
        }
    }




    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);




    __super::Attack_Input(fTimeDelta);
}

void CState_SwordMan_SpecialSkill_SwordTempest::Exit_State()
{
    if (!CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Lock_Fov())
        CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(Cam_Fov_Follow_Default);
}

CState_SwordMan_SpecialSkill_SwordTempest* CState_SwordMan_SpecialSkill_SwordTempest::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_SpecialSkill_SwordTempest* pInstance = new CState_SwordMan_SpecialSkill_SwordTempest(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_SpecialSkill_SwordTempest::Free()
{
    __super::Free();
}
