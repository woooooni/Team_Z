#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WindowWorldMap final : public CUI
{
protected:
	CUI_WindowWorldMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_WindowWorldMap(const CUI_WindowWorldMap& rhs);
	virtual ~CUI_WindowWorldMap() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	_bool Get_Active() { return m_bActive; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;
	virtual void On_MouseDragEnter(_float fTimeDelta) override;
	virtual void On_MouseDrag(_float fTimeDelta) override;
	virtual void On_MouseDragExit(_float fTimeDelta) override;

private:
	POINT m_ptMouse = {};
	_bool m_bMove = { false };

	_float m_fTimeAcc = { 0.f };
	_float2 m_vOffsetMin = _float2(0.f, 0.f);
	_float2 m_vOffsetMax = _float2(0.f, 0.f);
	
private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_WindowWorldMap* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
