#include "stdafx.h"
#include "GlanixState_RageIdle.h"

#include "Glanix.h"

CGlanixState_RageIdle::CGlanixState_RageIdle(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageIdle::Enter_State(void* pArg)
{
	if (m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp / 2.f && !m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pGlanix->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_BERSERK);
		return;
	}

	if (pArg != nullptr)
		m_fWaitTime = *(_float*)pArg;

	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* ������ look�� �÷��̾� ��ġ - ���� ��ġ�� ���� ���� ������ ������ ����. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* ������ �ٶ󺸴� ������ �������� �����ʿ� ��ġ. */
	if (fCrossProductY > 0.f)
	{
		m_fTurnSpeed = 2.f;
	}
	/* ������ �ٶ󺸴� ������ �������� ���ʿ� ��ġ. */
	else if (fCrossProductY < 0.f)
	{
		m_fTurnSpeed = -2.f;
	}
}

void CGlanixState_RageIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* ������ ������ ��� look �����ϰ� */
	m_vDestPos = m_pPlayerTransform->Get_Position();

	m_fTime += fTimeDelta;

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());

	/* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* ���������� �� */
	if (m_fTurnSpeed > 0.f)
	{
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}
	/* �������� �� */
	else if (m_fTurnSpeed < 0.f)
	{
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}

	if (m_fTime >= m_fWaitTime)
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGECHARGE);
	}
}

void CGlanixState_RageIdle::Exit_State()
{
}

CGlanixState_RageIdle* CGlanixState_RageIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageIdle* pInstance = new CGlanixState_RageIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageIdle::Free()
{
	__super::Free();
}
