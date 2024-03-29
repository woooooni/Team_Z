#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Skill_PerfectBlade.h"

#include "Effect_Manager.h"
#include "Camera.h"
#include "Camera_Manager.h"

CState_SwordMan_Skill_PerfectBlade::CState_SwordMan_Skill_PerfectBlade(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Skill_PerfectBlade::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Skill_PerfectBlade::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Swordsman_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    m_pCharacter->Look_For_Target();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_PerfectBlade"), pTransformCom->Get_WorldMatrix(), m_pCharacter);

}

void CState_SwordMan_Skill_PerfectBlade::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween())
    {
        /*if (m_pModelCom->Get_CurrAnimationFrame() == 15)
        {
            m_pCharacter->Look_For_Target();
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 20.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 22)
        {
            m_pCharacter->Look_For_Target();
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 20.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 28)
        {
            m_pCharacter->Look_For_Target();
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 20.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 34)
        {
            m_pCharacter->Look_For_Target();
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 20.f, fTimeDelta);
        }
        if (m_pModelCom->Get_CurrAnimationFrame() == 39)
        {
            m_pCharacter->Look_For_Target();
            m_pTransformCom->Move(-1.f * XMVector3Normalize(m_pTransformCom->Get_Right()), 20.f, fTimeDelta);
        }*/
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_SwordMan_Skill_PerfectBlade::Exit_State()
{
    if (!CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Lock_Fov())
        CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(Cam_Fov_Follow_Default);
}

CState_SwordMan_Skill_PerfectBlade* CState_SwordMan_Skill_PerfectBlade::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Skill_PerfectBlade* pInstance = new CState_SwordMan_Skill_PerfectBlade(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Skill_PerfectBlade::Free()
{
    __super::Free();
}
