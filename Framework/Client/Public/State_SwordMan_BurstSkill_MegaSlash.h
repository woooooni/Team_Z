#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_SwordMan_BurstSkill_MegaSlash : public CState_Character
{

private:
	CState_SwordMan_BurstSkill_MegaSlash(class CStateMachine* pMachine);
	~CState_SwordMan_BurstSkill_MegaSlash() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_bool m_bSlow = false;
	_bool m_bStopAction = false;
	_float m_fAccRadialBlur = 0.f;

	_float2 m_vScreenSpeed = { 0.f, 0.f };
private:
	void Start_Action();
	void Stop_Action();

public:
	static CState_SwordMan_BurstSkill_MegaSlash* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END