#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_SwordMan_SpecialSkill_SwordTempest : public CState_Character
{

private:
	CState_SwordMan_SpecialSkill_SwordTempest(class CStateMachine* pMachine);
	~CState_SwordMan_SpecialSkill_SwordTempest() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};

public:
	static CState_SwordMan_SpecialSkill_SwordTempest* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END