#pragma once

#include "Client_Defines.h"
#include "BehaviorTree.h"

BEGIN(Client)

class CClown;
class CClownBT final : public CBehaviorTree
{
private:
	CClownBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClownBT(const CClownBT& rhs);
	virtual ~CClownBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject* pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsZeroHp();
	_bool   IsHitAnim();
	_bool	IsAtk();
	_bool	IsChase();
	_bool	IsReturn();

private:
	CClown* m_pClown = nullptr;
	CBTNode::tagBTDesc m_tBTNodeDesc = {};

	_bool m_BIsChase = false;


public:
	static CClownBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
