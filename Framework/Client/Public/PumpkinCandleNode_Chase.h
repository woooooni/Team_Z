#pragma once

#include "PumpkinCandleNode_Base.h"

BEGIN(Client)

class CPumpkinCandleNode_Chase final : public CPumpkinCandleNode_Base
{
private:
	CPumpkinCandleNode_Chase();
	virtual ~CPumpkinCandleNode_Chase() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CPumpkinCandleNode_Chase* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

