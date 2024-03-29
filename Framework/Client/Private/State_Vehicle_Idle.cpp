#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "State_Vehicle_Idle.h"
#include "Vehicle.h"
#include "Game_Manager.h"
#include "Player.h"

CState_Vehicle_Idle::CState_Vehicle_Idle(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_Vehicle_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    // 우다닥만 사용하는 탈 것 상태
    
    return S_OK;
}

void CState_Vehicle_Idle::Enter_State(void* pArg)
{
//    if (false == m_pVehicle->Is_Aboard())
//        return;

    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

//    GI->Stop_Sound(CHANNELID::SOUND_VEHICLE);
//    _int iRandom = GI->RandomInt(0, 3);
//    switch (iRandom)
//    {
//    case 0:
//        GI->Play_Sound(TEXT("Veh_Udadak_Idle_1_1.mp3"), CHANNELID::SOUND_VEHICLE,
//            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
//        break;
//
//    case 1:
//        GI->Play_Sound(TEXT("Veh_Udadak_Idle_2_1.mp3"), CHANNELID::SOUND_VEHICLE,
//            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
//        break;
//
//    case 2:
//        GI->Play_Sound(TEXT("Veh_Udadak_Idle_2_2.mp3"), CHANNELID::SOUND_VEHICLE,
//            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
//        break;
//
//    case 3:
//        GI->Play_Sound(TEXT("Veh_Udadak_Idle_6.mp3"), CHANNELID::SOUND_VEHICLE,
//            GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
//        break;
//    }
}

void CState_Vehicle_Idle::Tick_State(_float fTimeDelta)
{
//    if (false == m_pVehicle->Is_Aboard())
//        return;

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
    {
        m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(0, m_AnimIndices.size() - 1)];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);

        
        _int iRandom = GI->RandomInt(0, 3);
        switch (iRandom)
        {
        case 0:
            GI->Play_Sound(TEXT("Veh_Udadak_Idle_1_1.mp3"), CHANNELID::SOUND_VEHICLE,
                GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
            break;

        case 1:
            GI->Play_Sound(TEXT("Veh_Udadak_Idle_2_1.mp3"), CHANNELID::SOUND_VEHICLE,
                GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
            break;

        case 2:
            GI->Play_Sound(TEXT("Veh_Udadak_Idle_2_2.mp3"), CHANNELID::SOUND_VEHICLE,
                GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
            break;

        case 3:
            GI->Play_Sound(TEXT("Veh_Udadak_Idle_6.mp3"), CHANNELID::SOUND_VEHICLE,
                GI->Get_ChannelVolume(CHANNELID::SOUND_VEHICLE));
            break;
        }
        
    }

    // 플레이어 움직임이 막혀있다면 return;
    if (false == CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Is_Move_Input())
        return;

	if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
	{
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_WALK);
		return;
	}

    if (KEY_TAP(KEY::SPACE))
    {
        CUI_Manager::GetInstance()->Use_JumpBtn();
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_JUMP);
        return;
    }

    if (true == GI->Mouse_Down(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(true);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(true);
    }

    if (true == GI->Mouse_Up(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(false);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(false);
    }
}

void CState_Vehicle_Idle::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    
    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);
}



CState_Vehicle_Idle* CState_Vehicle_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Vehicle_Idle* pInstance = new CState_Vehicle_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Vehicle_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Vehicle_Idle::Free()
{
    __super::Free();
}
