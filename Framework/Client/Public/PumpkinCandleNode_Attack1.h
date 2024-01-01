#pragma once

#include "PumpkinCandleNode_Base.h"

BEGIN(Client)

class CPumpkinCandleNode_Attack1 final : public CPumpkinCandleNode_Base
{
private:
	CPumpkinCandleNode_Attack1();
	virtual ~CPumpkinCandleNode_Attack1() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CPumpkinCandleNode_Attack1* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

