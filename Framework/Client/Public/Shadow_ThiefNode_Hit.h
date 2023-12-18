#pragma once

#include "Shadow_ThiefNode_Base.h"

BEGIN(Client)

class CShadow_ThiefNode_Hit final : public CShadow_ThiefNode_Base
{
private:
	CShadow_ThiefNode_Hit();
	virtual ~CShadow_ThiefNode_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CShadow_ThiefNode_Hit* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

