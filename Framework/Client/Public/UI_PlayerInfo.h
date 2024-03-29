#pragma once

#include "UI.h"

// PlayerStatus UI

BEGIN(Client)
class CUI_PlayerInfo final : public CUI
{
protected:
	CUI_PlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlayerInfo(const CUI_PlayerInfo& rhs);
	virtual ~CUI_PlayerInfo() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Hide_UI(_bool bHide);
	void Set_TextOnOff(_bool bOnOff) { m_bAddText = bOnOff; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bAddText = { false };

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vHidePosition = _float2(0.f, 0.f);

	_float2 m_vDefaultPosition = _float2(68.f, 31.f); // LevelText
	_float2 m_vTextPosition = _float2(145.f, 51.f);
	_float2 m_vNumPosition = _float2(200.f, 51.f);

	_float4 m_vLevelColor = { 0.82f, 0.773f, 0.627f, 1.f };
	_float4 m_vShadowColor = { 0.361f, 0.361f, 0.361f, 1.f };
	_float4 m_vPowerColor = { 0.525f, 0.871f, 0.749f, 1.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Movement_BasedOnHiding(_float fTimeDelta);
	

public:
	static CUI_PlayerInfo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
