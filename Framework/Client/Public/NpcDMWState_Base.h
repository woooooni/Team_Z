#pragma once

#include "Client_Defines.h"
#include "State.h"

#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "StateMachine.h"
#include "Navigation.h"
#include "RigidBody.h"

#include "Stellia.h"
#include "Character.h" 

BEGIN(Client)

class CDreamMazeWitch_Npc;
class CNpcDMWState_Base abstract : public CState
{
protected:
	CNpcDMWState_Base(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	void Following_Stellia(_float fTimeDelta);

	// 반짝이 함수 추가하기

protected:
	CCharacter* m_pPlayer = nullptr;
	CTransform* m_pPlayerTransform = nullptr;
	CGameObject* m_pStellia = nullptr;

	CDreamMazeWitch_Npc* m_pWitch = nullptr;

	_float m_fFollowingSpeed = 0.f;

	/* Damping */
	Vec4		 m_vCurPos = Vec4::Zero;
	const _float m_fDampingCoefficient = 0.027f; /* (144 프레임 기준 댐핑 계수 - 0과 1사이 범위를 갖으며, 값이 클수록 빨리 따라감)*/
	const _float m_fDampingLimitDistance = 1000.f;

	/* 공격 패턴 위한 변수들 */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;

	_bool m_bIsRage01 = false;
	_bool m_bIsRage02 = false;
	_bool m_bIsRage03 = false;

private:
	_float m_fTime = 0.f;
	_float m_fCreateParticleTime = 0.f;

public:
	virtual void Free() override;
};

END
