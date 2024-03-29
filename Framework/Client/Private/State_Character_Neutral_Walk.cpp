#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Character_Neutral_Walk.h"
#include "Particle_Manager.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_Character_Neutral_Walk::CState_Character_Neutral_Walk(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Walk::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Walk::Enter_State(void* pArg)
{
	m_pCharacter->Disappear_Weapon();
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

    if (CGame_Manager::GetInstance()->Get_Kuu() != nullptr)
        CGame_Manager::GetInstance()->Get_Kuu()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CKuu::NPC_UNIQUENPC_WALK);
}

void CState_Character_Neutral_Walk::Tick_State(_float fTimeDelta)
{
    GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fEffectAcc, CUtils::Random_Float(0.5f, 1.f), fTimeDelta, TEXT("Particle_Smoke"), m_pCharacter, _float3(0.f, 0.2f, 0.f));

    if (false == __super::Tag_Input(fTimeDelta))
    {
        __super::Neutral_Walk_Input(fTimeDelta);
        __super::Skill_Input(fTimeDelta);
    }
}

void CState_Character_Neutral_Walk::Exit_State()
{

}

CState_Character_Neutral_Walk* CState_Character_Neutral_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Walk* pInstance = new CState_Character_Neutral_Walk(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Walk");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Walk::Free()
{
    __super::Free();
}
