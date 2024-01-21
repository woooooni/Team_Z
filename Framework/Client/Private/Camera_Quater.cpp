#include "stdafx.h"
#include "Camera_Quater.h"

#include "GameInstance.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Player.h"

CCamera_Quater::CCamera_Quater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Quater::CCamera_Quater(const CCamera_Quater& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Quater::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Quater::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_VirtualTarget()))
		return E_FAIL;

	/* Set Camera */
	{
		Set_Fov(Cam_Fov_Quater_Default);
		Set_Distance(Cam_Dist_Quater_Default);	
	}
	return S_OK;
}

void CCamera_Quater::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (CCamera_Quater::MODE_TYPE::TRANSLATION == m_eModeType || CCamera_Quater::MODE_TYPE::NONE == m_eModeType)
		Tick_Input();

	switch (m_eModeType)
	{
	case CCamera_Quater::MODE_TYPE::TRANSLATION:
		Tick_Translation(fTimeDelta);
		break;
	case CCamera_Quater::MODE_TYPE::ZOOM:
		Tick_Zoom(fTimeDelta);
		break;
	case CCamera_Quater::MODE_TYPE::ROTATION:
		Tick_Rotation(fTimeDelta);
		break;
	default:
		break;
	}

	Test(fTimeDelta);
}

void CCamera_Quater::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

Vec4 CCamera_Quater::Get_LookAt()
{
	return Vec4::UnitW;
}

void CCamera_Quater::Tick_Blending(const _float fDeltaTime)
{
	const Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	const Vec4 vLook = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition);

	m_pTransformCom->LookAt(vLook);
}

void CCamera_Quater::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);

}

void CCamera_Quater::Set_Active(const _bool bActive)
{
	__super::Set_Active(bActive);

	if (m_bActive)
	{
		/* Player All Input Off */
		{
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_IDLE);

			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
		}

		/* ī�޶� �ٲ�� ���� �����ǰ� �� ���� (�Ҿƹ��� ����) */
		CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"TreeGrandfa");
		if (nullptr != pTarget)
		{
			CTransform* pTargetTransform = pTarget->Get_Component<CTransform>(L"Com_Transform");
			if(nullptr != pTargetTransform)
			{
				m_pVirtualTargetTransform->Set_WorldMatrix(pTargetTransform->Get_WorldMatrix());

				m_pVirtualTargetTransform->Set_State(CTransform::STATE::STATE_RIGHT, Vec4(m_pVirtualTargetTransform->Get_WorldMatrix().r[0]).Normalized());
				m_pVirtualTargetTransform->Set_State(CTransform::STATE::STATE_UP, Vec4(m_pVirtualTargetTransform->Get_WorldMatrix().r[1]).Normalized());
				m_pVirtualTargetTransform->Set_State(CTransform::STATE::STATE_LOOK, Vec4(m_pVirtualTargetTransform->Get_WorldMatrix().r[2]).Normalized());

				m_pVirtualTargetTransform->Set_State(CTransform::STATE_POSITION, pTargetTransform->Get_Position());
			}
		}

		/* ���� ���� ī�޶� Ʈ������ ���� */
		{
			/* Cam Position */
			{
				Vec4 vLookRight = Vec4(m_pVirtualTargetTransform->Get_Look() + m_pVirtualTargetTransform->Get_Right()).Normalized();

				Vec4 vGoalPos = (Vec4)m_pVirtualTargetTransform->Get_Position() /* Ÿ�� ���� ������ */
					+ vLookRight.ZeroY() * m_tLerpDist.fCurValue /* x, z ���� */
					+ Vec4::UnitY * m_fInitHeight; /* y ���� */

				m_vCurPos = vGoalPos.OneW();

				m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, m_vCurPos);
			}

			/* Cam LoookAt*/
			m_pTransformCom->LookAt(m_pVirtualTargetTransform->Get_Position());
		}

		/* Tick Transform */
		for (size_t i = 0; i < 15; i++)
		{
			Calculate_CamHeightFromDistance();
			Tick_Translation(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));
		}
	}
	else
	{
		/* Player All Input On */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
		m_eViewType = CCamera_Quater::VIEW_TYPE::NE;
		m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
		m_iMouseWheel = 0;
	}
}

HRESULT CCamera_Quater::Ready_Components()
{
	return S_OK;
}

HRESULT CCamera_Quater::Ready_VirtualTarget()
{
	m_pVirtualTargetTransform = dynamic_cast<CTransform*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Transform"));

	if (nullptr == m_pVirtualTargetTransform)
		return E_FAIL;

	return S_OK;
}

void CCamera_Quater::Tick_Input()
{
	/* Zoom */
	m_iMouseWheel = GI->Get_DIMMoveState(DIMM::DIMM_WHEEL);
	if (0 != m_iMouseWheel)
	{
		m_eModeType = MODE_TYPE::ZOOM;

		// ���� ����

		return;
	}

	/* Roatiton */
	{
		if (KEY_TAP(KEY::Q))
		{
			_int iType = (_int)m_eViewType + 1;
			if (VIEW_TYPE::NW < iType)
				iType = VIEW_TYPE::NE;

			m_eViewType = (CCamera_Quater::VIEW_TYPE)iType;
			m_eModeType = MODE_TYPE::ROTATION;

			m_tNextPosDesc.Start(m_pTransformCom->Get_Position(), Calculate_GoalPosition(), m_fRotLerpTime, LERP_MODE::SMOOTHER_STEP);

			// ���� ����
			return;
		}
		else if (KEY_TAP(KEY::E))
		{
			_int iType = (_int)m_eViewType - 1;
			if (iType < 0)
				iType = VIEW_TYPE::NW;

			m_eViewType = (CCamera_Quater::VIEW_TYPE)iType;
			m_eModeType = MODE_TYPE::ROTATION;

			m_tNextPosDesc.Start(m_pTransformCom->Get_Position(), Calculate_GoalPosition(), m_fRotLerpTime, LERP_MODE::SMOOTHER_STEP);

			// ���� ����
			return;
		}
	}

	/* Translation */
	{
		if (KEY_HOLD(KEY::W) && KEY_HOLD(KEY::D))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look()
				+ m_pTransformCom->Get_Right();
		}
		else if (KEY_HOLD(KEY::D) && KEY_HOLD(KEY::S))
		{
			m_vVirtualTargetMoveDir = (m_pTransformCom->Get_Look() * -1.f)
				+ m_pTransformCom->Get_Right();
		}
		else if (KEY_HOLD(KEY::S) && KEY_HOLD(KEY::A))
		{
			m_vVirtualTargetMoveDir = (m_pTransformCom->Get_Look() * -1.f)
				+ (m_pTransformCom->Get_Right() * -1.f);
		}
		else if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::W))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look()
				+ (m_pTransformCom->Get_Right() * -1.f);
		}
		else if (KEY_HOLD(KEY::W))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look();
		}
		else if (KEY_HOLD(KEY::S))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Look() * -1.f;
		}
		else if (KEY_HOLD(KEY::D))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Right();
		}
		else if (KEY_HOLD(KEY::A))
		{
			m_vVirtualTargetMoveDir = m_pTransformCom->Get_Right() * -1.f;
		}

		if (0.f < m_vVirtualTargetMoveDir.Length())
		{
			m_vVirtualTargetMoveDir.y = 0.f;
			m_vVirtualTargetMoveDir.Normalize();
			m_eModeType = MODE_TYPE::TRANSLATION;
		}
	}
}

void CCamera_Quater::Tick_Translation(const _float fDeltaTime)
{
	/* ���� Ÿ�� ���� �̵� */
	m_pVirtualTargetTransform->Translate(m_vVirtualTargetMoveDir * m_fVirtualTargetMoveSpeed * fDeltaTime);
	m_vVirtualTargetMoveDir = Vec3::Zero;

	/* Cam Position */
	{
		Vec4 vGoalPos = Calculate_GoalPosition();

		/*if (m_fDampingMaxDistance < Vec4::Distance(vGoalPos, m_vCurPos))
		{
			m_vCurPos = vGoalPos;
		}
		else
		{
			const Vec4 vDist = (vGoalPos.ZeroW() - m_vCurPos.ZeroW()) * m_fDampingCoefficient;
			m_vCurPos += vDist;
			m_vCurPos.y = vGoalPos.y;
		}*/
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vGoalPos.OneW());
	}

	/* Cam LoookAt*/
	m_pTransformCom->LookAt(m_pVirtualTargetTransform->Get_Position());

	/* ������ ������ ���¸� none���� ��ü�Ѵ�. */
	m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
}

void CCamera_Quater::Tick_Zoom(const _float fDeltaTime)
{
	if (0 < m_iMouseWheel) /* ���� */
	{
		--m_tLerpDist.fCurValue;
		if (m_tLerpDist.fCurValue < Cam_Dist_Quater_Min)
		{
			m_tLerpDist.fCurValue = Cam_Dist_Quater_Min;
			m_iMouseWheel = 0;
			m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
			return;
		}
	}
	else /* �� �ƿ� */
	{
		++m_tLerpDist.fCurValue;
		if (Cam_Dist_Quater_Max < m_tLerpDist.fCurValue)
		{
			m_tLerpDist.fCurValue = Cam_Dist_Quater_Max;
			m_iMouseWheel = 0;
			m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
			return;
		}
	}

	Calculate_CamHeightFromDistance();

	m_iMouseWheel = 0;
	
	/* �ӽ� */
	Tick_Translation(fDeltaTime);

	/* �� ������ ������ ���¸� None�� �ٲ۴�. */
	m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
}

void CCamera_Quater::Tick_Rotation(const _float fDeltaTime)
{
	if (m_tNextPosDesc.bActive)
		m_tNextPosDesc.Update_Lerp(fDeltaTime);

	/* �ӽ� */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tNextPosDesc.vCurVec);
	m_pTransformCom->LookAt(m_pVirtualTargetTransform->Get_Position());


	/* Sound */
	if (!m_bPlaySound && 0.3f <= m_tNextPosDesc.fCurTime / m_tNextPosDesc.fEndTime)
	{
		GI->Play_Sound(TEXT("Camera_moving.mp3"), CHANNELID::SOUND_CUTSCENE, 0.6f, true);
		m_bPlaySound = true;
	}


	/* ȸ�� ������ ������ ���¸� None�� �ٲ۴�. */
	if (!m_tNextPosDesc.bActive)
	{
		m_eModeType = CCamera_Quater::MODE_TYPE::NONE;
		m_bPlaySound = false;
	}
}

void CCamera_Quater::Test(_float fTimeDelta)
{
	if (KEY_TAP(KEY::INSERT))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
		{
			pFollowCam->Reset_WideView_To_DefaultView();
			pFollowCam->Set_Default_Position();
			CCamera_Manager::GetInstance()->Set_CurCamera(pFollowCam->Get_Key());
		}
	}
}

const _float& CCamera_Quater::Calculate_CamHeightFromDistance()
{
	/* ���� Ÿ���� �����ǰ� ī�޶��� ���������κ��� ī�޶��� ���� ���̸� ����Ѵ�. */

	Vec3 vDirVirtualTargetToCam = Vec3(m_pTransformCom->Get_Position() - m_pVirtualTargetTransform->Get_Position());
	Vec3 vDirXZ = vDirVirtualTargetToCam.ZeroY();

	vDirVirtualTargetToCam.Normalize();
	vDirXZ.Normalize();

	// acosf(vDirXZ.Dot(vDirVirtualTargetToCam)); �ϸ� ���� �ʹ� �۾���
	const _float fTheta = vDirXZ.Dot(vDirVirtualTargetToCam); 
	
	m_tHeight.fCurValue = (tanf(fTheta) * m_tLerpDist.fCurValue) * m_fHeightMag;

	return m_tHeight.fCurValue;
}

Vec4 CCamera_Quater::Calculate_GoalPosition()
{
	/* ���� Ÿ���� ��, ��, �����κ��� ī�޶� ��ġ�� �������� ����Ѵ�. */
	Vec4 vDir;

	switch (m_eViewType)
	{
	case CCamera_Quater::VIEW_TYPE::NE:
		vDir = (Vec4(m_pVirtualTargetTransform->Get_Look()) + Vec4(m_pVirtualTargetTransform->Get_Right())).ZeroW().Normalized();
		break;
	case CCamera_Quater::VIEW_TYPE::SE:
		vDir = (Vec4(m_pVirtualTargetTransform->Get_Look() * -1.f) + Vec4(m_pVirtualTargetTransform->Get_Right())).ZeroW().Normalized();
		break;
	case CCamera_Quater::VIEW_TYPE::SW:
		vDir = (Vec4(m_pVirtualTargetTransform->Get_Look() * -1.f) + Vec4(m_pVirtualTargetTransform->Get_Right() * -1.f)).ZeroW().Normalized();
		break;
	case CCamera_Quater::VIEW_TYPE::NW:
		vDir = (Vec4(m_pVirtualTargetTransform->Get_Look()) + Vec4(m_pVirtualTargetTransform->Get_Right() * -1.f)).ZeroW().Normalized();
		break;
	default:
		break;
	}

	Vec4 vGoalPos = (Vec4)m_pVirtualTargetTransform->Get_Position() /* Ÿ�� ���� ������ */
		+ vDir.ZeroY() * m_tLerpDist.fCurValue; /* x, z ���� */

	vGoalPos.w = 1.f;
	vGoalPos.y = m_tHeight.fCurValue;

	return vGoalPos;
}

CCamera_Quater* CCamera_Quater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Quater* pInstance = new CCamera_Quater(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Quater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Quater::Clone(void* pArg)
{
	CCamera_Quater* pInstance = new CCamera_Quater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Quater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Quater::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pVirtualTargetTransform);
}