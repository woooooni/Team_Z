#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_SwordMan_Battle_Attack_3 : public CState_Character
{

private:
	CState_SwordMan_Battle_Attack_3(class CStateMachine* pMachine);
	~CState_SwordMan_Battle_Attack_3() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_bool m_bGenTrail = false;
	_int m_iGenTrailCount = 0;

public:
	static CState_SwordMan_Battle_Attack_3* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END