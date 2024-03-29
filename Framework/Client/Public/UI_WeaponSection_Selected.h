#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_WeaponSection_Selected final : public CUI
{
protected:
	CUI_WeaponSection_Selected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_WeaponSection_Selected(const CUI_WeaponSection_Selected& rhs);
	virtual ~CUI_WeaponSection_Selected() = default;

public:
	void Hide_UI(_bool bHide) {
		m_bHide = bHide; m_bHideFinish = false;
		// TestCode
		if (bHide)
			m_bActive = false;
	}

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Update_Position(_uint iSlotNum);
	void Change_Weapon(_uint iSlotNum);
	_bool Isit_PossibleToChange(const wstring& strModelTag);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Movement_BasedOnHiding(_float fTimeDelta);

private:
	_bool m_bResize = { false };
	_float m_fSpeed = { 100.f };
	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMaxSize = _float2(0.f, 0.f);

	_float2 m_vPosition[3];

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);

	CHARACTER_TYPE m_ePreCharacter = { CHARACTER_END };

public:
	static CUI_WeaponSection_Selected* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
