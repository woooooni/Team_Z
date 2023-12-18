#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "PumpkinCandle.h"

BEGIN(Client)

class CPumpkinCandleNode_Base abstract : public CBTNode_Leaf
{
public:
	CPumpkinCandleNode_Base();
	virtual ~CPumpkinCandleNode_Base() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

	virtual CBTNode::NODE_STATE Atk_BehaviorTick(const wstring& strAnimName, _float fDestWaitTime, const _float& fTimeDelta);

	virtual _bool Behavior_Wait(_float fDestWaitTime, const _float& fTimeDelta);

protected:
	_bool	m_bWait = false;

private:
	_float  m_fSourWaitTime = 0.f;

public:
	virtual void Free() override;
};

END

