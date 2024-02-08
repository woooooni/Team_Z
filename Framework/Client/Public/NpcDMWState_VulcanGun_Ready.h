#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_VulcanGun_Ready final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_VulcanGun_Ready(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_VulcanGun_Ready() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsFindStellia = false;
	// 스텔리아의 5정도 떨어진 앞쪽에 내려간다.
	Vec4 m_vDestPos = {};
	_float m_fDist = 0.f;

	LERP_VEC4_DESC m_tRage03Lerp = {};
	_float m_fMoveTime = 0.f;

public:
	static CNpcDMWState_VulcanGun_Ready* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

