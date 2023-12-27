#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Engineer_Damaged_KnockDown : public CState_Character
{

private:
	CState_Engineer_Damaged_KnockDown(class CStateMachine* pMachine);
	~CState_Engineer_Damaged_KnockDown() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fAccRecovery = 0.f;
	_float m_fRecoveryTime = 3.f;

public:
	static CState_Engineer_Damaged_KnockDown* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END